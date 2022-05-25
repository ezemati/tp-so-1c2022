#include <kernel.h>

t_log *logger = NULL;
t_kernel_config *config = NULL;
t_list *lista_procesos = NULL;
t_list *lista_ready = NULL;

int main(int argc, char **argv)
{
	logger = log_create("cfg/kernel.log", "Kernel", true, LOG_LEVEL_TRACE);
	log_debug(logger, "Inicializando Kernel...");

	inicializar_kernel(argv);

	int socket_servidor = iniciar_servidor(config->puerto_escucha, logger);

	while (true)
	{
		int socket_cliente = esperar_cliente(socket_servidor);
		procesar_request(socket_cliente);
	}

	terminar_kernel();

	return 0;
}
