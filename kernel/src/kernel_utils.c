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

	liberar_conexion(socket_cliente);
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
