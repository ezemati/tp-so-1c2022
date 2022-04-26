#include "cpu.h"

t_log *logger;
t_cpu_config *config = NULL;

int main(int argc, char **argv)
{
	int conexion;
	int tamanioCadena = 50;

	logger = log_create("cfg/cpu.log", "CPU", true, LOG_LEVEL_TRACE);
	log_debug(logger, "Inicializando CPU...");

	inicializar_cpu(argv);

	//Me conecto a memoria
	log_info(logger, "Creando conexion con Memoria...");
	conexion = crear_conexion(config->ip_memoria, config->puerto_memoria, logger);

	if (conexion == -1)
	{
		log_error(logger, "No se pudo establecer la conexion con Memoria....\"\n");
		exit(1);
	}

	log_info(logger, "Enviando mensaje a Memoria...");
	char *cadena = (char *)malloc(tamanioCadena * sizeof(char));
	cadena = "Memoria, soy CPU...";
	int mensajeEnviado = enviar_string_por_socket(conexion, cadena);

	if(mensajeEnviado == -1)
	{
		log_error(logger, "No se pudo enviar el mensaje a Memoria....\"\n");
		exit(1);
	}

	log_info(logger, "CPU finalizando...");
	liberar_conexion(conexion);
	free(cadena);
	free(config);
}
