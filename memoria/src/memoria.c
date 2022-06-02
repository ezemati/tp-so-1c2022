#include <memoria.h>

t_log *logger = NULL;
t_memoria_config *config = NULL;
t_memoria_ram *memoria_ram = NULL;

int main(int argc, char **argv)
{
	if (argc == 2 && string_equals_ignore_case(argv[1], "--test"))
	{
		run_tests();
		return 0;
	}

	logger = log_create("cfg/memoria.log", "Memoria", true, LOG_LEVEL_TRACE);
	log_debug(logger, "Inicializando memoria...");

	inicializar_memoria(argv);

	int socket_servidor = iniciar_servidor(config->puerto_escucha, logger);
	if (socket_servidor == -1)
	{
		log_error_if_logger_not_null(logger, "Memoria no pudo crear el socket de servidor");
		exit(EXIT_FAILURE);
	}

	while (true)
	{
		int socket_cliente = esperar_cliente(socket_servidor);
		procesar_request(socket_cliente);
	}

	terminar_memoria();

	return 0;
}
