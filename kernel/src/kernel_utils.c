#include <kernel_utils.h>

static void handler_chequear_suspension_de_proceso_bloqueado(void *args);

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
	lista_blocked = list_create();

	pthread_mutex_init(&mutex_proximo_pid, NULL);
	pthread_mutex_init(&mutex_lista_procesos, NULL);
	pthread_mutex_init(&mutex_lista_ready, NULL);
	pthread_mutex_init(&mutex_lista_suspended_ready, NULL);
	pthread_mutex_init(&mutex_lista_new, NULL);
	pthread_mutex_init(&mutex_lista_blocked, NULL);

	sem_init(&sem_procesos_bloqueados, 0, 0);
}

void terminar_kernel()
{
	log_debug(logger, "Finalizando kernel...");
	log_destroy(logger);

	kernel_config_destroy(config);

	list_destroy(lista_ready);
	list_destroy(lista_suspended_ready);
	list_destroy(lista_new);
	list_destroy(lista_blocked);

	list_destroy_and_destroy_elements(lista_procesos, (void *)pcb_destroy);

	pthread_mutex_destroy(&mutex_lista_procesos);
	pthread_mutex_destroy(&mutex_lista_ready);
	pthread_mutex_destroy(&mutex_lista_suspended_ready);
	pthread_mutex_destroy(&mutex_lista_new);
	pthread_mutex_destroy(&mutex_lista_blocked);

	sem_destroy(&sem_procesos_bloqueados);

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
	uint32_t pid;

	pthread_mutex_lock(&mutex_proximo_pid);
	pid = proximo_pid;
	proximo_pid++;
	pthread_mutex_unlock(&mutex_proximo_pid);

	return pid;
}

bool puedo_pasar_proceso_a_memoria()
{
	uint32_t cantidad_procesos_en_memoria = calcular_multiprogramacion();
	log_trace_if_logger_not_null(logger, "Cantidad de procesos en memoria: %d/%d", cantidad_procesos_en_memoria, config->grado_multiprogramacion);
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

void bloquear_proceso(t_kernel_pcb *pcb, uint32_t tiempo_bloqueo)
{
	log_info_if_logger_not_null(logger, "Pasando proceso %d de %s a BLOCKED", pcb->id, estado_proceso_to_string(pcb->estado));

	pcb->estado = S_BLOCKED;
	pcb->bloqueo_pendiente = tiempo_bloqueo;
	pcb->milisegundos_en_running = 0;
	pcb->time_inicio_bloqueo = current_time();

	pthread_mutex_lock(&mutex_lista_blocked);
	list_add(lista_blocked, pcb);
	pthread_mutex_unlock(&mutex_lista_blocked);

	// Thread que chequea si el proceso estuvo en BLOCKED mas tiempo del permitido
	// y lo pasa a SUSPENDED_BLOCKED
	pthread_t thread_chequear_suspension_de_proceso_bloqueado_id;
	pthread_create(&thread_chequear_suspension_de_proceso_bloqueado_id, NULL, (void *)handler_chequear_suspension_de_proceso_bloqueado, pcb);
	pthread_detach(thread_chequear_suspension_de_proceso_bloqueado_id);

	sem_post(&sem_procesos_bloqueados);

	// Mandar un nuevo proceso a CPU (no quedo ninguno porque se bloqueo el
	// que se estaba ejecutando)
	replanificar();
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
	actualizar_pcb_desalojado(pcb, pcb_actualizado->program_counter, pcb_actualizado->time_inicio_running, pcb_actualizado->time_fin_running);

	agregar_proceso_a_ready_sin_replanificar(pcb);

	actualizarpcb_request_destroy(pcb_actualizado);
	free(response_serializada);
}

void enviar_proceso_a_cpu_para_ejecucion(t_kernel_pcb *pcb_a_ejecutar)
{
	log_info_if_logger_not_null(logger, "Pasando proceso %d de %s a RUNNING", pcb_a_ejecutar->id, estado_proceso_to_string(pcb_a_ejecutar->estado));

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

void handler_atencion_procesos_bloqueados()
{
	while (true)
	{
		sem_wait(&sem_procesos_bloqueados);

		pthread_mutex_lock(&mutex_lista_blocked);
		t_kernel_pcb *primer_proceso_en_blocked = list_get_first_element(lista_blocked);
		pthread_mutex_unlock(&mutex_lista_blocked);

		uint32_t milisegundos_io = primer_proceso_en_blocked->bloqueo_pendiente;
		uint32_t microsegundos_io = milisegundos_a_microsegundos(milisegundos_io);

		log_info_if_logger_not_null(logger, "Atendiendo I/O de proceso %d por %dms", primer_proceso_en_blocked->id, milisegundos_io);

		usleep(microsegundos_io);

		log_info_if_logger_not_null(logger, "Fin de I/O de proceso %d", primer_proceso_en_blocked->id);

		primer_proceso_en_blocked->bloqueo_pendiente = 0;

		pthread_mutex_lock(&mutex_lista_blocked);
		sacar_proceso_de_lista(lista_blocked, primer_proceso_en_blocked);
		pthread_mutex_unlock(&mutex_lista_blocked);

		if (primer_proceso_en_blocked->estado == S_BLOCKED)
		{
			agregar_proceso_a_ready(primer_proceso_en_blocked);
		}
		else if (primer_proceso_en_blocked->estado == S_SUSPENDED_BLOCKED)
		{
			agregar_proceso_a_suspended_ready(primer_proceso_en_blocked);
		}
	}
}

static void handler_chequear_suspension_de_proceso_bloqueado(void *args)
{
	t_kernel_pcb *pcb = args;

	// Me guardo el time de bloqueo para este bloqueo particular del proceso, en caso
	// de que el proceso salga de BLOCKED y vuelva a entrar mientras este hilo esta sleepeado
	time_t time_inicio_bloqueo = pcb->time_inicio_bloqueo;

	uint32_t milisegundos_maximos_de_bloqueo = config->tiempo_maximo_bloqueado;
	uint32_t microsegundos_maximos_de_bloqueo = milisegundos_a_microsegundos(milisegundos_maximos_de_bloqueo);

	// Sleepeo por la cantidad maxima de bloqueo, si al terminar el sleep() el proceso sigue
	// bloqueado, entonces lo suspendo
	usleep(microsegundos_maximos_de_bloqueo);

	if (pcb->estado == S_BLOCKED && times_son_iguales(pcb->time_inicio_bloqueo, time_inicio_bloqueo))
	{
		bool se_paso_proceso_a_memoria = false;
		suspender_proceso(pcb, &se_paso_proceso_a_memoria);
	}
}
