#include "kernel_utils.h"

void inicializar_kernel(char **argv)
{
	config = kernel_config_new("cfg/kernel.config", logger);
}

void terminar_kernel()
{
	log_debug(logger, "Finalizando kernel...");
	log_destroy(logger);

	kernel_config_destroy(config);
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
	case REQUEST_PROCESO_CONSOLA:
		request_proceso_consola(socket_cliente);
		break;
	default:
		enviar_string_con_longitud_por_socket(socket_cliente, "TEST: error");
		break;
	}

	liberar_conexion(socket_cliente);
	free(args);

	return NULL;
}
