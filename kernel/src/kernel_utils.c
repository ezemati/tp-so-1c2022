#include <kernel_utils.h>

void inicializar_kernel(int argc, char **argv)
{
	char *ruta_config = argc > 1
							? argv[1]
							: "cfg/kernel.config";
	config = kernel_config_new(ruta_config, logger);
	lista_procesos = list_create();
	lista_ready = list_create();
	lista_suspended_ready = list_create();
	lista_new = list_create();

	pthread_mutex_init(&mutex_lista_procesos, NULL);
	pthread_mutex_init(&mutex_lista_ready, NULL);
	pthread_mutex_init(&mutex_lista_suspended_ready, NULL);
	pthread_mutex_init(&mutex_lista_new, NULL);
}

void terminar_kernel()
{
	log_debug(logger, "Finalizando kernel...");
	log_destroy(logger);

	kernel_config_destroy(config);

	list_destroy(lista_ready);
	list_destroy(lista_suspended_ready);
	list_destroy(lista_new);

	list_destroy_and_destroy_elements(lista_procesos, (void *)pcb_destroy);

	pthread_mutex_destroy(&mutex_lista_procesos);
	pthread_mutex_destroy(&mutex_lista_ready);
	pthread_mutex_destroy(&mutex_lista_suspended_ready);
	pthread_mutex_destroy(&mutex_lista_new);

	liberar_conexion(socket_conexion_cpu_dispatch);
}

void procesar_request(int socket_cliente)
{
	int *socket_cliente_dup = malloc(sizeof(int));
	*socket_cliente_dup = socket_cliente;

	pthread_t thread_id;
	pthread_create(&thread_id, NULL, (void *)procesar_cliente, socket_cliente_dup);
	pthread_detach(thread_id);
}

void *procesar_cliente(void *args)
{
	int socket_cliente = *((int *)args);
	// log_debug(logger, "Cliente conectado por socket %d", socket_cliente);

	uint32_t id_op = -1;
	recibir_uint32_por_socket(socket_cliente, &id_op);

	log_info_if_logger_not_null(logger, "Operacion recibida en Kernel: %s", identificador_operacion_to_string(id_op));

	switch (id_op)
	{
	case CREAR_PROCESO:
		atender_crear_proceso(socket_cliente);
		break;
	default:
		enviar_string_con_longitud_por_socket(socket_cliente, "TEST: error");
		break;
	}

	// liberar_conexion(socket_cliente);

	free(args);

	return NULL;
}

uint32_t obtener_proximo_pid()
{
	pthread_mutex_lock(&mutex_lista_procesos);
	t_kernel_pcb *ultimo_proceso = list_get_last_element(lista_procesos);
	uint32_t proximo_pid = ultimo_proceso == NULL
							   ? 0 // Si no hay ningun proceso cargado, el primer PID es 0
							   : ultimo_proceso->id + 1;
	pthread_mutex_unlock(&mutex_lista_procesos);
	return proximo_pid;
}

bool puedo_pasar_proceso_a_memoria()
{
	uint32_t cantidad_procesos_en_memoria = calcular_multiprogramacion();
	log_trace_if_logger_not_null(logger, "Cantidad de procesos en memoria: %d", cantidad_procesos_en_memoria);
	return cantidad_procesos_en_memoria < config->grado_multiprogramacion;
}

int calcular_multiprogramacion()
{
	return cantidad_procesos_con_estado(S_READY) + cantidad_procesos_con_estado(S_RUNNING) + cantidad_procesos_con_estado(S_BLOCKED);
}

bool intentar_pasar_proceso_a_memoria()
{
	/**
	 * Hay que llamar a esta funcion cada vez que:
	 * - se agrega un proceso a NEW (para intentar que pase a READY)
	 * - un proceso pasa a EXIT (disminuye la multiprogramacion, hay un lugar mas)
	 * - un proceso pasa a SUSPENDED_BLOCKED (disminuye la multiprogramacion, hay un lugar mas)
	 * - un proceso pasa a SUSPENDED_READY (para intentar que pase a READY)
	 */

	// El PlanifMedianoPlazo tiene mas prioridad que el PlanifLargoPlazo
	bool mediano_plazo_pudo_pasar_proceso_a_memoria = mediano_plazo_intentar_pasar_proceso_a_memoria();
	bool largo_plazo_pudo_pasar_proceso_a_memoria = largo_plazo_intentar_pasar_proceso_a_memoria();
	return mediano_plazo_pudo_pasar_proceso_a_memoria || largo_plazo_pudo_pasar_proceso_a_memoria;
}

t_list *obtener_procesos_con_estado(estado_proceso estado)
{
	bool proceso_tiene_estado(void *element)
	{
		t_kernel_pcb *elementPcb = element;
		return elementPcb->estado == estado;
	}
	return list_filter(lista_procesos, proceso_tiene_estado);
}

t_kernel_pcb *obtener_proceso_por_pid(uint32_t pid)
{
	bool get_element(void *element)
	{
		t_kernel_pcb *elementPcb = element;
		return elementPcb->id == pid;
	}

	return list_find(lista_procesos, get_element);
}

uint32_t cantidad_procesos_con_estado(estado_proceso estado)
{
	bool proceso_tiene_estado(void *element)
	{
		t_kernel_pcb *elementPcb = element;
		return elementPcb->estado == estado;
	}
	return list_count_satisfying(lista_procesos, proceso_tiene_estado);
}

void sacar_proceso_de_lista(t_list *lista, t_kernel_pcb *pcb)
{
	bool remove_element(void *element)
	{
		t_kernel_pcb *elementPcb = element;
		return elementPcb->id == pcb->id;
	}

	list_remove_by_condition(lista, remove_element);
}

void bloquear_o_suspender_proceso(t_kernel_pcb *pcb, uint32_t tiempo_bloqueo)
{
	pcb->bloqueo_pendiente = tiempo_bloqueo;
	pcb->milisegundos_en_running = 0;

	if (tiempo_bloqueo > config->tiempo_maximo_bloqueado)
	{
		bool se_paso_proceso_a_memoria = false;
		suspender_proceso(pcb, &se_paso_proceso_a_memoria);
		if (!se_paso_proceso_a_memoria)
		{
			// Si se pudo pasar un proceso a memoria, entonces se replanifica automaticamente,
			// asi que hay que replanificar aca a mano unicamente si no se pudo pasar ningun proceso
			replanificar();
		}
	}
	else
	{
		bloquear_proceso(pcb);
		// Al bloquear un proceso no se intenta pasar un proceso a memoria,
		// asi que hay que replanificar a mano
		replanificar();
	}
}

void recalcular_estimacion(t_kernel_pcb *pcb)
{
	double alfa = config->alfa;
	double estimacion_anterior = pcb->estimacion_rafaga;
	double real_anterior = pcb->milisegundos_en_running;
	double nueva_estimacion = alfa * real_anterior + (1 - alfa) * estimacion_anterior;
	pcb->estimacion_rafaga = nueva_estimacion;

	log_trace_if_logger_not_null(logger, "Recalculando estimacion para proceso %d", pcb->id);
	log_trace_if_logger_not_null(logger, "{ EstimacionAnterior=%f, RealAnterior=%f }", estimacion_anterior, real_anterior);
	log_info_if_logger_not_null(logger, "Nueva estimacion para el proceso %d: %f", pcb->id, nueva_estimacion);
}

void print_instrucciones(t_kernel_pcb *pcb)
{
	t_list_iterator *iterator = list_iterator_create(pcb->lista_instrucciones);
	while (list_iterator_has_next(iterator))
	{
		t_instruccion *instruccion = list_iterator_next(iterator);
		char *format_instruccion = format_instruccion_para_print(instruccion);
		log_trace_if_logger_not_null(logger, "Instruccion: %s", format_instruccion);
		free(format_instruccion);
	}
	list_iterator_destroy(iterator);
}

void print_instrucciones_de_todos_los_procesos(t_list *pcbs)
{
	t_list_iterator *iterator = list_iterator_create(pcbs);
	while (list_iterator_has_next(iterator))
	{
		t_kernel_pcb *pcb = list_iterator_next(iterator);
		log_trace_if_logger_not_null(logger, "PID %d", pcb->id);
		print_instrucciones(pcb);
	}
	list_iterator_destroy(iterator);
}

void enviar_interrupcion_a_cpu()
{
	log_info_if_logger_not_null(logger, "Enviando interrupcion a CPU para desalojar proceso");

	int socket_interrupt_cpu = crear_conexion(config->ip_cpu, config->puerto_cpu_interrupt, NULL);

	uint32_t interrumpir_ejecucion = 1;
	enviar_uint32_por_socket(socket_interrupt_cpu, interrumpir_ejecucion);

	void *response_serializada = NULL;
	recibir_buffer_con_bytes_por_socket(socket_interrupt_cpu, &response_serializada);
	t_kernel_actualizarpcb_request *pcb_actualizado = deserializar_actualizarpcb_request(response_serializada);

	hay_proceso_en_ejecucion = false;

	t_kernel_pcb *pcb = obtener_proceso_por_pid(pcb_actualizado->pid);
	pcb->program_counter = pcb_actualizado->program_counter;
	cargar_tiempo_ejecucion_en_cpu(pcb, pcb_actualizado->time_inicio_running, pcb_actualizado->time_fin_running);
	pcb->estado = S_READY;
	pthread_mutex_lock(&mutex_lista_ready);
    list_add(lista_ready, pcb);
    pthread_mutex_unlock(&mutex_lista_ready);

	actualizarpcb_request_destroy(pcb_actualizado);
	free(response_serializada);
}

void enviar_proceso_a_cpu_para_ejecucion(t_kernel_pcb *pcb_a_ejecutar)
{
	log_info_if_logger_not_null(logger, "Enviado proceso %d a CPU para ejecutar", pcb_a_ejecutar->id);

	int socket_dispatch_cpu = crear_conexion(config->ip_cpu, config->puerto_cpu_dispatch, NULL);

	t_cpu_ejecutarproceso_request *request = ejecutarproceso_request_new(pcb_a_ejecutar->id, pcb_a_ejecutar->tamanio, pcb_a_ejecutar->program_counter, pcb_a_ejecutar->tabla_paginas_primer_nivel, pcb_a_ejecutar->lista_instrucciones);
	int bytes_request_serializada;
	void *request_serializada = serializar_ejecutarproceso_request(request, &bytes_request_serializada);
	enviar_buffer_serializado_con_instruccion_y_bytes_por_socket(socket_dispatch_cpu, EJECUTAR_PROCESO, request_serializada, bytes_request_serializada);
	free(request_serializada);
	ejecutarproceso_request_destroy(request);

	void *response_serializada = NULL;
	recibir_buffer_con_bytes_por_socket(socket_dispatch_cpu, &response_serializada);
	t_cpu_ejecutarproceso_response *response = deserializar_ejecutarproceso_response(response_serializada);
	bool ok = response->ok;
	ejecutarproceso_response_destroy(response);
	free(response_serializada);

	liberar_conexion(socket_dispatch_cpu);

	if (!ok)
	{
		log_error_if_logger_not_null(logger, "Error al enviar nuevo proceso para ejecucion (PID %d) a CPU", pcb_a_ejecutar->id);
	}

	pcb_a_ejecutar->estado = S_RUNNING;
	hay_proceso_en_ejecucion = true;
}
