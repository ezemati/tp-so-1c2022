#include "consola.h"
#include "utils.h"
#include "tests.h"

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/txt.h>

t_log *logger = NULL;

char *ip_kernel = NULL;
char *puerto_kernel = NULL;

char *ruta_programa = NULL;
uint32_t tamanio_programa = 0;

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

	FILE *file_programa = fopen(ruta_programa, "r");
	if (file_programa == NULL)
	{
		log_error(logger, "No se pudo abrir el archivo especificado (%s)", ruta_programa);
		return EXIT_FAILURE;
	}
	t_list *instrucciones = leer_instrucciones(file_programa);
	fclose(file_programa);

	int bytes;
	void *instrucciones_serializadas = serializar_instrucciones(instrucciones, tamanio_programa, &bytes);
	int socket_kernel = crear_conexion_con_kernel(ip_kernel, puerto_kernel);
	enviar_instrucciones(socket_kernel, instrucciones_serializadas, bytes);

	free(instrucciones_serializadas);
	instrucciones_destroy(instrucciones);
	terminar_consola();
	return 0;
}

void inicializar_consola(char **argv)
{
	ruta_programa = argv[1];
	tamanio_programa = strtoul(argv[2], NULL, 10);
	log_info(logger, "{ RUTA = %s, TAMANIO = %d }", ruta_programa, tamanio_programa);

	t_config *config = config_create("cfg/consola.config");
	ip_kernel = string_duplicate(config_get_string_value(config, "IP_KERNEL"));
	puerto_kernel = string_duplicate(config_get_string_value(config, "PUERTO_KERNEL"));
	config_destroy(config);

	log_info(logger, "{ IP_KERNEL = %s, PUERTO_KERNEL = %s }", ip_kernel, puerto_kernel);
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
			t_instruccion *instruccion = malloc(sizeof(t_instruccion));
			instruccion->codigo_instruccion = string_duplicate(codigo_instruccion);
			instruccion->cantidad_parametros = 0;
			list_add(instrucciones, instruccion);
		}
	}
	else
	{
		t_instruccion *instruccion = malloc(sizeof(t_instruccion));

		instruccion->codigo_instruccion = string_duplicate(codigo_instruccion);

		if (string_equals_ignore_case(instruccion->codigo_instruccion, "EXIT"))
		{
			instruccion->cantidad_parametros = 0;
		}
		else if (string_equals_ignore_case(instruccion->codigo_instruccion, "NO_OP") || string_equals_ignore_case(instruccion->codigo_instruccion, "I/O") || string_equals_ignore_case(instruccion->codigo_instruccion, "READ"))
		{
			instruccion->cantidad_parametros = 1;
			instruccion->parametros[0] = strtoul(linea_spliteada[1], NULL, 10);
		}
		else if (string_equals_ignore_case(instruccion->codigo_instruccion, "WRITE") || string_equals_ignore_case(instruccion->codigo_instruccion, "COPY"))
		{
			instruccion->cantidad_parametros = 2;
			instruccion->parametros[0] = strtoul(linea_spliteada[1], NULL, 10);
			instruccion->parametros[1] = strtoul(linea_spliteada[2], NULL, 10);
		}
		else
		{
			log_error(logger, "Instruccion no reconocida: %s", instruccion->codigo_instruccion);
			exit(EXIT_FAILURE);
		}

		list_add(instrucciones, instruccion);
	}

	string_array_destroy(linea_spliteada);
}

int crear_conexion_con_kernel(char *ip, char *puerto)
{
	struct addrinfo hints, *servinfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &servinfo);

	int socket_kernel = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	if (socket_kernel == -1)
	{
		log_error(logger, "Error al crear el socket con el Kernel");
		exit(EXIT_FAILURE);
	}

	int connect_result = connect(socket_kernel, servinfo->ai_addr, servinfo->ai_addrlen);
	if (connect_result == -1)
	{
		log_error(logger, "Fallo la conexion con el socket del Kernel");
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(servinfo);

	return socket_kernel;
}

void enviar_instrucciones(int socket_kernel, void *instrucciones_serializadas, int bytes)
{
	send(socket_kernel, instrucciones_serializadas, bytes, 0);
}

void terminar_consola()
{
	log_debug(logger, "Finalizando consola");
	log_destroy(logger);

	free(ip_kernel);
	free(puerto_kernel);
}

void instrucciones_destroy(t_list *instrucciones)
{
	list_destroy_and_destroy_elements(instrucciones, instruccion_destroy);
}

void instruccion_destroy(void *buffer_instruccion)
{
	t_instruccion *instruccion = (t_instruccion *)buffer_instruccion;
	free(instruccion->codigo_instruccion);
	free(instruccion);
}
