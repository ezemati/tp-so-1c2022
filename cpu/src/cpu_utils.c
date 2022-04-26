#include <cpu_utils.h>

void inicializar_cpu(char **argv)
{
	config = cpu_config_new("cfg/cpu.config", logger);
}

void terminar_cpu()
{
	log_debug(logger, "Finalizando CPU...");
	log_destroy(logger);

	cpu_config_destroy(config);
}
