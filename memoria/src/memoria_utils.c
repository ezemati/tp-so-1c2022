#include <memoria_utils.h>

void inicializar_memoria(char **argv)
{
	config = memoria_config_new("cfg/memoria.config", logger);
}

void terminar_memoria()
{
	log_debug(logger, "Finalizando memoria");
	log_destroy(logger);

	memoria_config_destroy(config);
}

void procesar_request(int socket_cliente)
{
	uint32_t *socket_cliente_dup = malloc(sizeof(uint32_t));
	*socket_cliente_dup = socket_cliente; // Hay que duplicar el socket_cliente porque si no pasan cosas mamadas en el procesar_cliente

	pthread_t thread_id;
	pthread_create(&thread_id, NULL, procesar_cliente, socket_cliente_dup);
	pthread_detach(thread_id); // Para que el hilo principal no se quede esperando a que el hilo handler termine
}

void *procesar_cliente(void *args)
{
	uint32_t socket_cliente = *((uint32_t *)args);
	log_debug(logger, "Cliente conectado por socket %d", socket_cliente);

	uint32_t id_op = -1;
	recibir_uint32_por_socket(socket_cliente, &id_op);

	switch (id_op)
	{
	case INICIALIZAR_PROCESO:
		inicializar_proceso(socket_cliente);
		break;

	default:
		enviar_uint32_por_socket(socket_cliente, -1);
		break;
	}

	liberar_conexion(socket_cliente);
	free(args);

	return NULL;
}

void inicializar_proceso(int socket_cliente)
{
	enviar_string_por_socket(socket_cliente, "TEST: inicializando proceso...\n");
}
