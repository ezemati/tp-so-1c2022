#include <consola.h>

t_log *logger = NULL;
t_consola_config *config = NULL;
t_programa *programa = NULL;

int main(int argc, char **argv)
{
	if (argc == 2 && string_equals_ignore_case(argv[1], "--test"))
	{
		run_tests();
		return 0;
	}

	logger = log_create("cfg/consola.log", "Consola", true, LOG_LEVEL_TRACE);
	log_debug(logger, "Inicializando consola");

	if (argc < 3)
	{
		log_error(logger, "Cantidad de parametros incorrecta (%d)", argc);
		log_error(logger, "Formato: ./bin/consola.out PATH_PROGRAMA TAMANIO [PATH_CONFIG]");
		log_destroy(logger);
		return EXIT_FAILURE;
	}

	inicializar_consola(argc, argv);

	int bytes;
	void *programa_serializado = serializar_programa(programa, &bytes);
	int socket_kernel = crear_conexion(config->ip_kernel, config->puerto_kernel, logger);
	if (socket_kernel == -1)
	{
		log_error(logger, "Consola no pudo conectarse a Kernel");
		terminar_consola();
		exit(EXIT_FAILURE);
	}

	enviar_buffer_serializado_con_instruccion_y_bytes_por_socket(socket_kernel, CREAR_PROCESO, programa_serializado, bytes);
	uint32_t response;
	recibir_uint32_por_socket(socket_kernel, &response);
	if (response == 1)
	{
		log_info_if_logger_not_null(logger, "Conexion con Kernel finalizada correctamente");
	}
	else
	{
		log_error_if_logger_not_null(logger, "Ocurrio un error en la conexion con Kernel");
	}

	liberar_conexion(socket_kernel);

	free(programa_serializado);

	terminar_consola();
	return 0;
}
