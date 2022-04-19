#include "consola.h"

t_log *logger = NULL;
t_consola_config *config = NULL;

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

	FILE *file_programa = fopen(config->ruta_programa, "r");
	if (file_programa == NULL)
	{
		log_error(logger, "No se pudo abrir el archivo especificado (%s)", config->ruta_programa);
		return EXIT_FAILURE;
	}
	t_list *instrucciones = leer_instrucciones(file_programa);
	fclose(file_programa);

	int bytes;
	void *instrucciones_serializadas = serializar_instrucciones(instrucciones, config->tamanio_programa, &bytes);
	int socket_kernel = crear_conexion(config->ip_kernel, config->puerto_kernel, logger);
	enviar_por_socket(socket_kernel, instrucciones_serializadas, bytes);

	free(instrucciones_serializadas);
	instrucciones_destroy(instrucciones);
	terminar_consola();
	return 0;
}

void inicializar_consola(char **argv)
{
	config = consola_config_new(argv, "cfg/consola.config", logger);
}

t_list *leer_instrucciones(FILE *archivo)
{
	t_list *instrucciones = list_create();

	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	while ((read = getline(&line, &len, archivo)) != -1)
	{
		string_trim(&line);
		log_debug(logger, "Linea leida: `%s`", line);

		agregar_instruccion(line, instrucciones);
	}

	free(line);

	return instrucciones;
}

void agregar_instruccion(char *linea, t_list *instrucciones)
{
	char **linea_spliteada = string_split(linea, " ");
	char *codigo_instruccion = linea_spliteada[0];

	if (string_equals_ignore_case(codigo_instruccion, "NO_OP"))
	{
		int cantidad_noop = strtoul(linea_spliteada[1], NULL, 10);
		for (int i = 1; i <= cantidad_noop; i++)
		{
			t_instruccion *instruccion = instruccion_new(codigo_instruccion, NULL);
			list_add(instrucciones, instruccion);
		}
	}
	else
	{
		// Los parametros empiezan en la segunda posicion del linea_spliteada (por eso el `+ 1`)
		t_instruccion *instruccion = instruccion_new(codigo_instruccion, linea_spliteada + 1);
		list_add(instrucciones, instruccion);
	}

	string_array_destroy(linea_spliteada);
}

void terminar_consola()
{
	log_debug(logger, "Finalizando consola");
	log_destroy(logger);
	
	consola_config_destroy(config);
}
