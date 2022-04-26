#include "cpu.h"

t_log *logger;
t_cpu_config *config = NULL;

int main(int argc, char **argv)
{
	logger = log_create("cfg/cpu.log", "CPU", true, LOG_LEVEL_TRACE);
	log_debug(logger, "Inicializando CPU...");

	inicializar_cpu(argv);

	//Me conecto a memoria
	log_info(logger, "Creando conexion con Memoria...");
	int conexion = crear_conexion(config->ip_memoria, config->puerto_memoria, logger);

	if (conexion == -1)
	{
		log_error(logger, "No se pudo establecer la conexion con Memoria....\"\n");
		exit(1);
	}

	log_info(logger, "Enviando mensaje a Memoria...");
	enviar_uint32_por_socket(conexion, INICIALIZAR_PROCESO);
	
	char *cadena = NULL;
	recibir_string_con_longitud_por_socket(conexion, &cadena);
	log_info(logger, "La Memoria me respondio: %s", cadena);

	log_info(logger, "CPU finalizando...");
	liberar_conexion(conexion);
	free(cadena);
	free(config);
}
