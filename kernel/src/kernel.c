#include <kernel.h>

int main(int argc, char **argv) {

	t_log * logger;
	t_config * config;

	int kernel_client_memoria_fd;
	int kernel_client_cpu_fd;
	int kernel_server_consola_fd;
	int consola_cliente_kernel_fd;

	logger = iniciar_logger();
	logger = log_create("kernel.log", "Kernel", 1, LOG_LEVEL_DEBUG);
	log_info(logger, "Iniciando Kernel...");

	if (argc < 2){
		log_error(logger, "Falta parametro de archivo de configuracion. Ejemplo: ./kernel.out \"/home/utnso/tp-2022-1c-ElProtocoloMesi/kernel/cfg/kernel.config\"\n");
		exit(1);
	}

	config = iniciar_config();
	config = config_create(argv[1]);
	char * ip_memoria = config_get_string_value(config, "IP_MEMORIA");
	char * puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
	char * ip_cpu = config_get_string_value(config, "IP_CPU");
	char * puerto_cpu_dispatch = config_get_string_value(config,"PUERTO_CPU_DISPATCH");
	char * puerto_cpu_interrupt = config_get_string_value(config,"PUERTO_CPU_INTERRUPT");

	//Creamos una conexion a la Memoria
	log_info(logger, "Creando conexion con Memoria...");
	kernel_client_memoria_fd = crear_conexion(ip_memoria, puerto_memoria, logger);

	//Enviamos un mensaje a la Memoria
	log_info(logger, "Enviando mensaje a Memoria...");
	int tamanioCadena = 50;
	char * cadena = (char*) malloc(tamanioCadena * sizeof(char));
	cadena = "Memoria, soy Kernel!!";
	enviar_mensaje(cadena, kernel_client_memoria_fd);

	//Creamos conexion a la CPU
	log_info(logger, "Creando conexion con CPU...");
	kernel_client_cpu_fd = crear_conexion(ip_cpu, puerto_cpu_dispatch, logger);

	//Enviamos un mensaje a la CPU
	log_info(logger, "Enviando mensaje a CPU...");
	cadena = "CPU, soy Kernel!!";
	enviar_mensaje(cadena, kernel_client_cpu_fd);

	//Kernel como Servidor de Consola
	log_info(logger, "Esperando conexion con Consola...");
	char * puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");
	kernel_server_consola_fd = iniciar_servidor(puerto_escucha, logger);
	log_info(logger, "Kernel listo para recibir a Consola");
	consola_cliente_kernel_fd = esperar_cliente(kernel_server_consola_fd);

	terminar_programa(kernel_client_memoria_fd, logger, config);
	terminar_programa(kernel_client_cpu_fd, logger, config);

}

t_config* iniciar_config(void) {
	t_config* nuevo_config;
	return nuevo_config;
}

void terminar_programa(int conexion, t_log* logger, t_config* config) {
	liberar_conexion(conexion);
	log_destroy(logger);
	config_destroy(config);
}

