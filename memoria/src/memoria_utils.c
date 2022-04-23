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
	pthread_t thread_id;
	pthread_create(&thread_id, NULL, procesar_cliente, &socket_cliente);
	pthread_detach(thread_id);
}

void *procesar_cliente(void *args)
{
	int socket_cliente = *((int *)args);
	log_debug(logger, "Socket Cliente: %d", socket_cliente);
	
	char *buffer = "Probando conexion                                  :)";
	enviar_por_socket(socket_cliente,buffer, strlen(buffer)+1);

	liberar_conexion(socket_cliente);
	
	return NULL;
}
