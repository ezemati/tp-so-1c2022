#include <cpu.h>

t_log *logger = NULL;
t_cpu_config *config = NULL;
t_cpu_info_ejecucion_actual *info_ejecucion_actual = NULL;

bool hay_interrupcion = false;
int socket_conexion_kernel = -1;

int main(int argc, char **argv)
{
	if (argc == 2 && string_equals_ignore_case(argv[1], "--test"))
	{
		run_tests();
		return 0;
	}

	logger = log_create("cfg/cpu.log", "CPU", true, LOG_LEVEL_TRACE);
	log_debug(logger, "Inicializando CPU...");

	inicializar_cpu(argv);

	realizar_handshake_con_memoria(config);

	pthread_t thread_interrupt_id;
	pthread_create(&thread_interrupt_id, NULL, (void *)interrupt_listener, NULL);
	pthread_detach(thread_interrupt_id);

	int socket_servidor_dispatch = iniciar_servidor(config->puerto_escucha_dispatch, logger);
	if (socket_servidor_dispatch == -1)
	{
		log_error_if_logger_not_null(logger, "CPU no pudo crear el socket de servidor de dispatch");
		exit(EXIT_FAILURE);
	}

	while (true)
	{
		int socket_cliente_dispatch = esperar_cliente(socket_servidor_dispatch);
		procesar_request(socket_cliente_dispatch);
	}

	terminar_cpu();

	return 0;
}

void *interrupt_listener(void *args)
{
	int socket_servidor_interrupt = iniciar_servidor(config->puerto_escucha_interrupt, logger);
	if (socket_servidor_interrupt == -1)
	{
		log_error_if_logger_not_null(logger, "CPU no pudo crear el socket de servidor de interrupt");
		exit(EXIT_FAILURE);
	}

	while (true)
	{
		int socket_cliente_interrupt = esperar_cliente(socket_servidor_interrupt);

		uint32_t recibido;
		recibir_uint32_por_socket(socket_cliente_interrupt, &recibido);
		if (recibido == 1)
		{
			hay_interrupcion = true;
		}

		liberar_conexion(socket_cliente_interrupt);
	}

	return NULL;
}
