#include <kernel_utils.h>

static t_kernel_pcb *obtener_proximo_para_ejecutar_fifo();
static t_kernel_pcb *obtener_proximo_para_ejecutar_srt();
static bool algoritmo_es_con_desalojo();
static void enviar_interrupcion_a_cpu();
static void enviar_nuevo_proceso_a_cpu(t_kernel_pcb *pcb_a_ejecutar);
static void thread_proceso_blocked(void *args);
static void thread_proceso_suspended_blocked(void *args);

void inicializar_kernel(char **argv)
{
	config = kernel_config_new("cfg/kernel.config", logger);
	lista_procesos = list_create();
	lista_ready = list_create();
	lista_suspended_ready = list_create();
	lista_new = list_create();
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
}

void procesar_request(int socket_cliente)
{
	uint32_t *socket_cliente_dup = malloc(sizeof(uint32_t));
	*socket_cliente_dup = socket_cliente;

	pthread_t thread_id;
	pthread_create(&thread_id, NULL, (void *)procesar_cliente, socket_cliente_dup);
	pthread_detach(thread_id);
}

void *procesar_cliente(uint32_t *args)
{
	uint32_t socket_cliente = *args;
	log_debug(logger, "Cliente conectado por socket %d", socket_cliente);

	uint32_t id_op = -1;
	recibir_uint32_por_socket(socket_cliente, &id_op);

	switch (id_op)
	{
	case CREAR_PROCESO:
		crear_proceso(socket_cliente);
		break;
	default:
		enviar_string_con_longitud_por_socket(socket_cliente, "TEST: error");
		break;
	}

	if (id_op != CREAR_PROCESO)
	{
		// Si la operacion es CREAR_PROCESO, hay que dejar la conexion viva porque despues
		// hay que avisarle a la Consola cuando el proceso termine la ejecucion
		liberar_conexion(socket_cliente);
	}
	free(args);

	return NULL;
}

uint32_t obtener_proximo_pid()
{
	t_kernel_pcb *ultimo_proceso = (t_kernel_pcb *)list_get_last_element(lista_procesos);
	return ultimo_proceso == NULL
			   ? 0 // Si no hay ningun proceso cargado, el primer PID es 0
			   : ultimo_proceso->id + 1;
}

bool puedo_pasar_proceso_a_memoria()
{
	uint32_t cantidad_procesos_en_memoria = cantidad_procesos_con_estado(READY) + cantidad_procesos_con_estado(RUNNING) + cantidad_procesos_con_estado(BLOCKED);
	log_trace_if_logger_not_null(logger, "Cantidad de procesos en memoria: %d", cantidad_procesos_en_memoria);
	return cantidad_procesos_en_memoria < config->grado_multiprogramacion;
}

void pasar_proceso_new_a_ready(t_kernel_pcb *pcb)
{
	int socket_memoria = crear_conexion(config->ip_memoria, config->puerto_memoria, logger);

	t_memoria_inicializarproceso_request *request = inicializarproceso_request_new(pcb->id, pcb->tamanio);
	int bytes_request_serializada = 0;
	void *request_serializada = serializar_inicializarproceso_request(request, &bytes_request_serializada);
	enviar_buffer_serializado_con_instruccion_y_bytes_por_socket(socket_memoria, INICIALIZAR_PROCESO, request_serializada, bytes_request_serializada);
	free(request_serializada);
	inicializarproceso_request_destroy(request);

	void *response_serializada = NULL;
	recibir_buffer_con_bytes_por_socket(socket_memoria, &response_serializada);
	t_memoria_inicializarproceso_response *response = deserializar_inicializarproceso_response(response_serializada);
	pcb->tabla_paginas_primer_nivel = response->numero_tablaprimernivel;
	pcb->estado = READY;
	agregar_proceso_a_ready(pcb);
	inicializarproceso_response_destroy(response);
	free(response_serializada);

	liberar_conexion(socket_memoria);

	log_info_if_logger_not_null(logger, "Proceso %d pasado a READY, con numero de tabla de primer nivel %d", pcb->id, pcb->tabla_paginas_primer_nivel);
}

void intentar_pasar_proceso_a_memoria()
{
	/**
	 * Hay que llamar a esta funcion cada vez que:
	 * - se agrega un proceso a NEW (para intentar que pase a READY)
	 * - un proceso pasa a EXIT (disminuye la multiprogramacion, hay un lugar mas)
	 * - un proceso pasa a SUSPENDED_BLOCKED (disminuye la multiprogramacion, hay un lugar mas)
	 * - un proceso pasa a SUSPENDED_READY (para intentar que pase a READY)
	 */

	if (!puedo_pasar_proceso_a_memoria())
	{
		return;
	}

	t_kernel_pcb *pcb_suspended_ready = list_get_first_element(lista_suspended_ready);
	if (pcb_suspended_ready != NULL)
	{
		sacar_proceso_de_lista(lista_suspended_ready, pcb_suspended_ready);
		log_info_if_logger_not_null(logger, "Pasando proceso %d desde SUSPENDED_READY a READY", pcb_suspended_ready->id);
		agregar_proceso_a_ready(pcb_suspended_ready);
		return;
	}

	t_kernel_pcb *pcb_new = list_get_first_element(lista_new);
	if (pcb_new != NULL)
	{
		sacar_proceso_de_lista(lista_new, pcb_new);
		log_info_if_logger_not_null(logger, "Pasando proceso %d desde NEW a READY", pcb_new->id);
		pasar_proceso_new_a_ready(pcb_new);
	}
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

uint32_t cantidad_procesos_con_estado(estado_proceso estado)
{
	bool proceso_tiene_estado(void *element)
	{
		t_kernel_pcb *elementPcb = element;
		return elementPcb->estado == estado;
	}
	return list_count_satisfying(lista_procesos, proceso_tiene_estado);
}

void finalizar_proceso(t_kernel_pcb *pcb)
{
	pcb->estado = EXIT;
	finalizar_proceso_en_memoria(pcb);
	finalizar_proceso_en_consola(pcb);
	sacar_proceso_de_lista(lista_procesos, pcb);
	pcb_destroy(pcb);

	intentar_pasar_proceso_a_memoria();
}

void finalizar_proceso_en_memoria(t_kernel_pcb *pcb)
{
	int socket_memoria = crear_conexion(config->ip_memoria, config->puerto_memoria, logger);

	t_memoria_finalizarproceso_request *request = finalizarproceso_request_new(pcb->id, pcb->tabla_paginas_primer_nivel);
	int bytes_request_serializada = 0;
	void *request_serializada = serializar_finalizarproceso_request(request, &bytes_request_serializada);
	enviar_buffer_serializado_con_instruccion_y_bytes_por_socket(socket_memoria, FINALIZAR_PROCESO, request_serializada, bytes_request_serializada);
	free(request_serializada);
	finalizarproceso_request_destroy(request);
}

void finalizar_proceso_en_consola(t_kernel_pcb *pcb)
{
	uint32_t proceso_finalizado_ok = 1;
	enviar_uint32_por_socket(pcb->socket_consola, proceso_finalizado_ok);
	liberar_conexion(pcb->socket_consola);
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

void agregar_proceso_a_ready(t_kernel_pcb *pcb)
{
	pcb->estado = READY;
	list_add(lista_ready, pcb);

	if (algoritmo_es_con_desalojo())
	{
		replanificar();
	}
}

void replanificar()
{
	enviar_interrupcion_a_cpu();

	t_kernel_pcb *pcb_a_ejecutar = obtener_proximo_para_ejecutar();
	sacar_proceso_de_lista(lista_ready, pcb_a_ejecutar);
	// pcb_a_ejecutar->estado = RUNNING;

	enviar_nuevo_proceso_a_cpu(pcb_a_ejecutar);
}

t_kernel_pcb *obtener_proximo_para_ejecutar()
{
	t_kernel_pcb *pcb_a_ejecutar = string_equals_ignore_case(config->algoritmo_planificacion, "FIFO")
									   ? obtener_proximo_para_ejecutar_fifo()
									   : obtener_proximo_para_ejecutar_srt();
	return pcb_a_ejecutar;
}

void bloquear_o_suspender_proceso(t_kernel_pcb *pcb, uint32_t tiempo_bloqueo)
{
	pcb->bloqueo_pendiente = tiempo_bloqueo;
	if (tiempo_bloqueo > config->tiempo_maximo_bloqueado)
	{
		suspender_proceso(pcb);
	}
	else
	{
		bloquear_proceso(pcb);
	}
}

void bloquear_proceso(t_kernel_pcb *pcb)
{
	pcb->estado = BLOCKED;
	pthread_t thread_id;
	pthread_create(&thread_id, NULL, (void *)thread_proceso_blocked, pcb);
	pthread_detach(thread_id);
}

void suspender_proceso(t_kernel_pcb *pcb)
{
	pcb->estado = SUSPENDED_BLOCKED;

	int socket_memoria = crear_conexion(config->ip_memoria, config->puerto_memoria, logger);

	t_memoria_suspenderproceso_request *request = suspenderproceso_request_new(pcb->id, pcb->tabla_paginas_primer_nivel);
	int bytes = 0;
	void *request_serializada = serializar_suspenderproceso_request(request, &bytes);
	enviar_buffer_serializado_con_instruccion_y_bytes_por_socket(socket_memoria, SUSPENDER_PROCESO, request_serializada, bytes);
	free(request_serializada);
	suspenderproceso_request_destroy(request);

	void *response_serializada = NULL;
	recibir_buffer_con_bytes_por_socket(socket_memoria, &response_serializada);
	t_memoria_suspenderproceso_response *response = deserializar_suspenderproceso_response(response_serializada);
	if (!response->ok)
	{
		log_error_if_logger_not_null(logger, "Fallo memoria al suspender al proceso %d", pcb->id);
	}
	else
	{
		intentar_pasar_proceso_a_memoria(); // Este proceso paso a SUSPENDED_BLOCKED asi que dejo un espacio de multiprogramacion libre

		pthread_t thread_id;
		pthread_create(&thread_id, NULL, (void *)thread_proceso_suspended_blocked, pcb);
		pthread_detach(thread_id);
	}
	suspenderproceso_response_destroy(response);
	free(response_serializada);

	liberar_conexion(socket_memoria);
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

static t_kernel_pcb *obtener_proximo_para_ejecutar_fifo()
{
	return list_get_first_element(lista_ready);
}

static t_kernel_pcb *obtener_proximo_para_ejecutar_srt()
{
	void *pcb_con_estimacion_minima(void *element1, void *element2)
	{
		t_kernel_pcb *elementPcb1 = element1;
		t_kernel_pcb *elementPcb2 = element2;
		return elementPcb1->estimacion_rafaga <= elementPcb2->estimacion_rafaga
				   ? elementPcb1
				   : elementPcb2;
	}
	return list_get_minimum(lista_ready, pcb_con_estimacion_minima);
}

static bool algoritmo_es_con_desalojo()
{
	return string_equals_ignore_case(config->algoritmo_planificacion, "SRT");
}

static void enviar_interrupcion_a_cpu()
{
	// TODO: tener en cuenta que CPU tiene que mandar el PCB actualizado, y Kernel tiene que actualizar los datos
}

static void enviar_nuevo_proceso_a_cpu(t_kernel_pcb *pcb_a_ejecutar)
{
	// TODO
}

static void thread_proceso_blocked(void *args)
{
	t_kernel_pcb *pcb = args;
	int tiempo_bloqueo = pcb->bloqueo_pendiente;
	int microsegundos = tiempo_bloqueo / 1000; // tiempo_bloqueo esta en milisegundos
	log_info_if_logger_not_null(logger, "Proceso %d entrando en bloqueo por %dms", pcb->id, tiempo_bloqueo);

	usleep(microsegundos);

	log_info_if_logger_not_null(logger, "Proceso %d saliendo de bloqueo y pasando a READY", pcb->id);
	pcb->bloqueo_pendiente = 0;
	agregar_proceso_a_ready(pcb);
}

static void thread_proceso_suspended_blocked(void *args)
{
	t_kernel_pcb *pcb = args;
	int tiempo_bloqueo = pcb->bloqueo_pendiente;
	int microsegundos = tiempo_bloqueo / 1000; // tiempo_bloqueo esta en milisegundos
	log_info_if_logger_not_null(logger, "Proceso %d entrando en suspension por %dms", pcb->id, tiempo_bloqueo);

	usleep(microsegundos);

	log_info_if_logger_not_null(logger, "Proceso %d saliendo de suspension", pcb->id);
	pcb->bloqueo_pendiente = 0;
	pcb->estado = SUSPENDED_READY;
	list_add(lista_suspended_ready, pcb);
}
