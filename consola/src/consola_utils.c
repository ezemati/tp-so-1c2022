#include <consola_utils.h>

void inicializar_consola(int argc, char **argv)
{
	char *ruta_config = argc > 3
							? argv[3]
							: "cfg/consola.config";

	config = consola_config_new(ruta_config, logger);

	pthread_mutex_init(&mutex_logger, NULL);
	programa = programa_new(argv[1], string_to_long(argv[2]), logger, &mutex_logger);
}

void terminar_consola()
{
	log_debug(logger, "Finalizando consola");
	log_destroy(logger);

	consola_config_destroy(config);
	programa_destroy(programa);

	pthread_mutex_destroy(&mutex_logger);
}
