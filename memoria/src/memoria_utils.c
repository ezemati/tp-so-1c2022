#include <memoria_utils.h>

static bool request_viene_de_cpu(identificador_operacion id_op);
static void bloquear_hilo_por_retardo_memoria_con_cpu();

void inicializar_memoria(int argc, char **argv)
{
	char *ruta_config = argc > 1
							? argv[1]
							: "cfg/memoria.config";
	config = memoria_config_new(ruta_config);
	memoria_ram = memoria_ram_new();

	sem_init(&sem_swap_libre, 0, 1);

	char command_crear_carpeta_swap[1024];
	sprintf(command_crear_carpeta_swap, "mkdir -p %s", config->path_swap);
	system(command_crear_carpeta_swap);
}

void terminar_memoria()
{
	log_info(logger, "Finalizando memoria...");
	log_destroy(logger);

	memoria_config_destroy(config);

	sem_destroy(&sem_swap_libre);

	memoria_ram_destroy(memoria_ram);
}

void procesar_request(int socket_cliente)
{
	uint32_t *socket_cliente_dup = malloc(sizeof(*socket_cliente_dup));
	*socket_cliente_dup = socket_cliente; // Hay que duplicar el socket_cliente porque si no pasan cosas mamadas en el procesar_cliente

	pthread_t thread_id;
	pthread_create(&thread_id, NULL, (void *)procesar_cliente, socket_cliente_dup);
	pthread_detach(thread_id); // Para que el hilo principal no se quede esperando a que el hilo handler termine
}

void *procesar_cliente(uint32_t *args)
{
	uint32_t socket_cliente = *args;
	// log_debug(logger, "Cliente conectado por socket %d", socket_cliente);

	uint32_t id_op = -1;
	recibir_uint32_por_socket(socket_cliente, &id_op);

	log_info(logger, "Operacion recibida en Memoria: %s", identificador_operacion_to_string(id_op));

	if (request_viene_de_cpu(id_op))
	{
		bloquear_hilo_por_retardo_memoria_con_cpu();
	}

	switch (id_op)
	{
	case INICIALIZAR_PROCESO:
		inicializar_proceso(socket_cliente);
		break;
	case FINALIZAR_PROCESO:
		finalizar_proceso(socket_cliente);
		break;
	case SUSPENDER_PROCESO:
		suspender_proceso(socket_cliente);
		break;
	case HANDSHAKE_INFO_TRADUCCION:
		handshake_info_traduccion(socket_cliente);
		break;
	case LEER_DATO:
		leer_dato(socket_cliente);
		break;
	case ESCRIBIR_DATO:
		escribir_dato(socket_cliente);
		break;
	case OBTENER_NUMERO_TABLA_2_PARA_ENTRADA_TABLA_1:
		obtener_numero_tabla_2_para_entrada_tabla_1(socket_cliente);
		break;
	case OBTENER_MARCO_PARA_ENTRADA_TABLA_2:
		obtener_marco_para_entrada_tabla_2(socket_cliente);
		break;
	default:
		enviar_string_con_longitud_por_socket(socket_cliente, "TEST: error");
		break;
	}

	log_info(logger, "Operacion finalizada en Memoria: %s", identificador_operacion_to_string(id_op));

	liberar_conexion(socket_cliente);
	free(args);

	return NULL;
}

static bool request_viene_de_cpu(identificador_operacion id_op)
{
	return id_op == HANDSHAKE_INFO_TRADUCCION || id_op == LEER_DATO || id_op == ESCRIBIR_DATO || id_op == OBTENER_NUMERO_TABLA_2_PARA_ENTRADA_TABLA_1 || id_op == OBTENER_MARCO_PARA_ENTRADA_TABLA_2;
}

static void bloquear_hilo_por_retardo_memoria_con_cpu()
{
	log_trace(logger, "Empezando RETARDO_MEMORIA (%dms)", config->retardo_memoria);

	time_microseg microsegundos_retardo_memoria = milisegundos_a_microsegundos(config->retardo_memoria);
	usleep(microsegundos_retardo_memoria);

	log_trace(logger, "RETARDO_MEMORIA finalizado");
}
