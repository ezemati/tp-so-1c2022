#include <kernel.h>

t_kernel_config *config = NULL;
t_log *logger = NULL;
t_list *lista_procesos = NULL;
t_list *lista_ready = NULL;
t_list *lista_suspended_ready = NULL;
t_list *lista_new = NULL;

int socket_conexion_cpu_dispatch = -1;
bool hay_proceso_en_ejecucion = false;

pthread_mutex_t mutex_lista_procesos;
pthread_mutex_t mutex_lista_ready;
pthread_mutex_t mutex_lista_suspended_ready;
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

	socket_conexion_cpu_dispatch = crear_conexion(config->ip_cpu, config->puerto_cpu_dispatch, logger);
	uint32_t ok_enviado = 1, ok_recibido = 0;
	enviar_uint32_por_socket(socket_conexion_cpu_dispatch, HANDSHAKE_SOY_KERNEL);
	enviar_uint32_por_socket(socket_conexion_cpu_dispatch, ok_enviado);
	recibir_uint32_por_socket(socket_conexion_cpu_dispatch, &ok_recibido);
	if (ok_recibido != 1)
	{
		log_error_if_logger_not_null(logger, "No pude establecer conexion con CPU Dispatch");
		exit(EXIT_FAILURE);
	}

	pthread_t thread_escucha_cpu_dispatch_id;
	pthread_create(&thread_escucha_cpu_dispatch_id, NULL, (void *)handler_escucha_cpu_dispatch, NULL);
	pthread_detach(thread_escucha_cpu_dispatch_id);

	int socket_servidor = iniciar_servidor(config->puerto_escucha, logger);
	if (socket_servidor == -1)
	{
		log_error_if_logger_not_null(logger, "Kernel no pudo crear el socket de servidor");
		exit(EXIT_FAILURE);
	}

	while (true)
	{
		int socket_cliente = esperar_cliente(socket_servidor);
		procesar_request(socket_cliente);
	}

	terminar_kernel();

	return 0;
}

void handler_escucha_cpu_dispatch()
{
	while (true)
	{
		uint32_t id_op = 0;
		recibir_uint32_por_socket(socket_conexion_cpu_dispatch, &id_op);

		log_info_if_logger_not_null(logger, "Operacion recibida de CPU Dispatch: %s", identificador_operacion_to_string(id_op));

		switch (id_op)
		{
		case BLOQUEAR_PROCESO:
			atender_bloquear_proceso(socket_conexion_cpu_dispatch);
			break;
		case FINALIZAR_PROCESO:
			atender_finalizar_proceso(socket_conexion_cpu_dispatch);
			break;
		default:
			break;
		}
	}
}
