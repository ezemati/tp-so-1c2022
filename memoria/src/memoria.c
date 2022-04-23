#include "memoria.h"

t_log *logger = NULL;
t_memoria_config *config = NULL;

int main(int argc, char **argv)
{
	logger = log_create("cfg/memoria.log", "Memoria", true, LOG_LEVEL_TRACE);
	log_debug(logger, "Inicializando memoria...");

	inicializar_memoria(argv);

	int socket_servidor = iniciar_servidor(config->puerto_escucha, logger);

	while (true)
	{
		int socket_cliente = esperar_cliente(socket_servidor);
		procesar_request(socket_cliente);
	}

	return 0;
}
