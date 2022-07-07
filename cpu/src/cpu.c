#include <cpu.h>

t_log *logger = NULL;
t_cpu_config *config = NULL;
t_cpu_info_ejecucion_actual *info_ejecucion_actual = NULL;
t_cpu_tlb *tlb = NULL;

bool hay_interrupcion = false;
int socket_conexion_kernel_dispatch = -1;
int socket_conexion_kernel_interrupt = -1;

int main(int argc, char **argv)
{
	if (argc == 2 && string_equals_ignore_case(argv[1], "--test"))
	{
		run_tests();
		return 0;
	}

	char *file_name = get_log_file_name("cpu");
	logger = log_create(file_name, "CPU", true, LOG_LEVEL_TRACE);
	free(file_name);

	log_debug(logger, "Inicializando CPU...");

	inicializar_cpu(argc, argv);

	realizar_handshake_con_memoria();

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

void *interrupt_listener()
{
	int socket_servidor_interrupt = iniciar_servidor(config->puerto_escucha_interrupt, logger);
	if (socket_servidor_interrupt == -1)
	{
		log_error_if_logger_not_null(logger, "CPU no pudo crear el socket de servidor de interrupt");
		exit(EXIT_FAILURE);
	}

	while (true)
	{
		socket_conexion_kernel_interrupt = esperar_cliente(socket_servidor_interrupt);

		uint32_t recibido;
		recibir_uint32_por_socket(socket_conexion_kernel_interrupt, &recibido);
		if (recibido == 1)
		{
			log_info_if_logger_not_null(logger, "Interrupcion recibida en CPU");
			hay_interrupcion = true;
			// El PCB actualizado se manda al chequear interrupciones en el ciclo de ejecucion
		}

		// liberar_conexion(socket_cliente_interrupt);
	}

	return NULL;
}
