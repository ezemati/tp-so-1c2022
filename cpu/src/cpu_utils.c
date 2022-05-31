#include <cpu_utils.h>

void inicializar_cpu(char **argv)
{
	config = cpu_config_new("cfg/cpu.config", logger);
}

void terminar_cpu()
{
	log_debug(logger, "Finalizando CPU...");
	log_destroy(logger);

	cpu_config_destroy(config);
}

void procesar_request(int socket_cliente)
{
	int *socket_cliente_dup = malloc(sizeof(int));
	*socket_cliente_dup = socket_cliente;

	pthread_t thread_id;
	pthread_create(&thread_id, NULL, (void *)procesar_cliente, socket_cliente_dup);
	pthread_detach(thread_id);
}

void *procesar_cliente(void *args)
{
	int socket_cliente = *((int *)args);
	log_debug(logger, "Cliente conectado por socket %d", socket_cliente);

	uint32_t id_op = -1;
	recibir_uint32_por_socket(socket_cliente, &id_op);

	switch (id_op)
	{
	case EJECUTAR_PROCESO:
		atender_ejecutar_proceso(socket_cliente);
		break;
	default:
		enviar_string_con_longitud_por_socket(socket_cliente, "TEST: error");
		break;
	}

	// liberar_conexion(socket_cliente);

	free(args);

	return NULL;
}

void realizar_handshake_con_memoria(t_cpu_config *config)
{
	log_info_if_logger_not_null(logger, "Realizando handshake con Memoria");

	int socket_memoria = crear_conexion(config->ip_memoria, config->puerto_memoria, logger);

	enviar_uint32_por_socket(socket_memoria, HANDSHAKE_INFO_TRADUCCION);

	void *buffer_response = NULL;
	recibir_buffer_con_bytes_por_socket(socket_memoria, &buffer_response);

	t_memoria_handshakeconfiguraciones_response *response = deserializar_handshakeconfiguraciones_memoria_response(buffer_response);
	config->memoria_entradas_por_tabla = response->entradas_por_tabla;
	config->memoria_tamanio_pagina = response->tamanio_pagina;

	log_info_if_logger_not_null(logger, "Handshake finalizado:- entradas_por_tabla = %d, tamanio_pagina = %d", config->memoria_entradas_por_tabla, config->memoria_tamanio_pagina);

	handshakeconfiguraciones_memoria_response_destroy(response);
	free(buffer_response);
	liberar_conexion(socket_memoria);
}
