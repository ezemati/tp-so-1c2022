#include <memoria.h>

t_log *logger = NULL;
t_memoria_config *config = NULL;
t_memoria_ram *memoria_ram = NULL;

pthread_mutex_t mutex_logger;
sem_t sem_swap_libre;

int main(int argc, char **argv)
{
	if (argc == 2 && string_equals_ignore_case(argv[1], "--test"))
	{
		run_tests();
		return 0;
	}

	char *file_name = get_log_file_name("memoria");
	logger = log_create(file_name, "Memoria", true, LOG_LEVEL_TRACE);
	free(file_name);

	log_info(logger, "Inicializando memoria...");

	inicializar_memoria(argc, argv);

	int socket_servidor = iniciar_servidor(config->puerto_escucha, logger, &mutex_logger);
	if (socket_servidor == -1)
	{
		log_error_with_mutex(logger, &mutex_logger, "Memoria no pudo crear el socket de servidor");
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
