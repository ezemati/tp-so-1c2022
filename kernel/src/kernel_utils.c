#include <kernel_utils.h>

void inicializar_kernel(char **argv)
{
	config = kernel_config_new("cfg/kernel.config", logger);
	lista_procesos = list_create();
}

void terminar_kernel()
{
	log_debug(logger, "Finalizando kernel...");
	log_destroy(logger);

	kernel_config_destroy(config);

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

	void *buffer_response_serializada = NULL;
	recibir_buffer_con_bytes_por_socket(socket_memoria, &buffer_response_serializada);
	t_memoria_inicializarproceso_response *response_deserializada = deserializar_inicializarproceso_response(buffer_response_serializada);
	pcb->tabla_paginas_primer_nivel = response_deserializada->numero_tablaprimernivel;
	pcb->estado = READY;
	inicializarproceso_response_destroy(response_deserializada);
	free(buffer_response_serializada);

	liberar_conexion(socket_memoria);

	log_info_if_logger_not_null(logger, "Proceso %d pasado a READY, con numero de tabla de primer nivel %d", pcb->id, pcb->tabla_paginas_primer_nivel);
}

uint32_t cantidad_procesos_con_estado(estado_proceso estado)
{
	uint32_t cantidad_con_estado = 0;
	t_list_iterator *iterator = list_iterator_create(lista_procesos);
	while (list_iterator_has_next(iterator))
	{
		t_kernel_pcb *pcb = list_iterator_next(iterator);
		if (pcb->estado == estado)
		{
			cantidad_con_estado++;
		}
	}
	list_iterator_destroy(iterator);
	return cantidad_con_estado;
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
