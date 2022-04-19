#include <consola_utils.h>

void inicializar_consola(char **argv)
{
	config = consola_config_new("cfg/consola.config", logger);
	programa = programa_new(argv[1], strtoul(argv[2], NULL, 10));
}

void terminar_consola()
{
	log_debug(logger, "Finalizando consola");
	log_destroy(logger);

	consola_config_destroy(config);
	programa_destroy(programa);
}
