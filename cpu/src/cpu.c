#include "cpu.h"

t_log *logger;
t_cpu_config *config = NULL;

int main(int argc, char **argv)
{
	logger = log_create("cfg/cpu.log", "CPU", true, LOG_LEVEL_TRACE);
	log_debug(logger, "Inicializando CPU...");

	inicializar_cpu(argv);

	realizar_handshake_con_memoria(config);

	while (true)
	{
		// TODO: ver aca como deberia ser (la CPU tiene que escuchar por dos puertos a la vez)
		break;
	}

	terminar_cpu();
}
