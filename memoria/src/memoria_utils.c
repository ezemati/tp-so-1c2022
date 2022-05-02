#include <memoria_utils.h>

void inicializar_memoria(char **argv)
{
	config = memoria_config_new("cfg/memoria.config", logger);
	memoria_ram = memoria_ram_new();
}

void terminar_memoria()
{
	log_debug(logger, "Finalizando memoria...");
	log_destroy(logger);

	memoria_config_destroy(config);
}

void procesar_request(int socket_cliente)
{
	uint32_t *socket_cliente_dup = malloc(sizeof(uint32_t));
	*socket_cliente_dup = socket_cliente; // Hay que duplicar el socket_cliente porque si no pasan cosas mamadas en el procesar_cliente

	pthread_t thread_id;
	pthread_create(&thread_id, NULL, (void *)procesar_cliente, socket_cliente_dup);
	pthread_detach(thread_id); // Para que el hilo principal no se quede esperando a que el hilo handler termine
}

void *procesar_cliente(uint32_t *args)
{
	uint32_t socket_cliente = *args;
	log_debug(logger, "Cliente conectado por socket %d", socket_cliente);

	uint32_t id_op = -1;
	recibir_uint32_por_socket(socket_cliente, &id_op);

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

	liberar_conexion(socket_cliente);
	free(args);

	return NULL;
}
