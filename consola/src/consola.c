#include "consola.h"

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

	if (argc != 3)
	{
		log_error(logger, "Cantidad de parametros incorrecta (%d)", argc);
		log_destroy(logger);
		return EXIT_FAILURE;
	}

	inicializar_consola(argv);

	int bytes;
	void *programa_serializado = serializar_programa(programa, &bytes);
	int socket_kernel = crear_conexion(config->ip_kernel, config->puerto_kernel, logger);
	if (socket_kernel == -1)
	{
		log_error(logger, "Consola no pudo conectarse a Kernel");
	}
	else
	{
		enviar_por_socket(socket_kernel, programa_serializado, bytes);
		liberar_conexion(socket_kernel);
	}
	free(programa_serializado);

	terminar_consola();
	return 0;
}
