#include <kernel.h>

t_kernel_config *config = NULL;
t_log *logger = NULL;
t_list *lista_procesos = NULL;
t_list *lista_ready = NULL;
t_list *lista_suspended_ready = NULL;
t_list *lista_new = NULL;

pthread_mutex_t mutex_lista_new;

int main(int argc, char **argv)
{
	if (argc == 2 && string_equals_ignore_case(argv[1], "--test"))
	{
		run_tests();
		return 0;
	}

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
