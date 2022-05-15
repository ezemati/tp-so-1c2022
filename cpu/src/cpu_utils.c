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

void realizar_handshake_con_memoria(t_cpu_config *config)
{
	log_info_if_logger_not_null(logger, "Realizando handshake con Memoria");

	int socket_memoria = crear_conexion(config->ip_memoria, config->puerto_memoria, logger);

	enviar_uint32_por_socket(socket_memoria, HANDSHAKE_INFO_TRADUCCION);

	uint32_t bytes_response;
	recibir_uint32_por_socket(socket_memoria, &bytes_response);
	void *buffer_response = malloc(bytes_response);
	recibir_buffer_por_socket(socket_memoria, buffer_response, bytes_response);

	t_memoria_handshakeconfiguraciones_response *response = deserializar_handshakeconfiguraciones_memoria_response(buffer_response);
	config->memoria_entradas_por_tabla = response->entradas_por_tabla;
	config->memoria_tamanio_pagina = response->tamanio_pagina;

	log_info_if_logger_not_null(logger, "Handshake finalizado:- entradas_por_tabla = %d, tamanio_pagina = %d", config->memoria_entradas_por_tabla, config->memoria_tamanio_pagina);

	handshakeconfiguraciones_memoria_response_destroy(response);
	free(buffer_response);
	liberar_conexion(socket_memoria);
}
