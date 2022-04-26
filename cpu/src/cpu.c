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
	int conexion_memoria = crear_conexion(config->ip_memoria, config->puerto_memoria, logger);

	if (conexion_memoria == -1)
	{
		log_error(logger, "No se pudo establecer la conexion con Memoria....\"\n");
		exit(1);
	}

	log_info(logger, "Enviando mensaje a Memoria...");
	enviar_uint32_por_socket(conexion_memoria, INICIALIZAR_PROCESO);

	char *cadena = NULL;
	recibir_string_con_longitud_por_socket(conexion_memoria, &cadena);
	log_info(logger, "La Memoria me respondio: %s", cadena);
	free(cadena);

	liberar_conexion(conexion_memoria);

	terminar_cpu();
}
