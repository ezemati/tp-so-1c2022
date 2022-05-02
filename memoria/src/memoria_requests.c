#include <memoria_requests.h>

void inicializar_proceso(int socket_cliente)
{
	uint32_t bytes_request;
	recibir_uint32_por_socket(socket_cliente, &bytes_request);

	void *buffer_request = malloc(bytes_request);
	recibir_por_socket(socket_cliente, buffer_request, bytes_request);

	t_memoria_inicializarproceso_request *request = deserializar_inicializarproceso_request(buffer_request);

	log_debug(logger, "Inicializando estructuras para proceso %d con tamanio %d", request->pid, request->tamanio_proceso);
	uint32_t numero_tablaprimernivel = memoria_ram_agregar_proceso(memoria_ram, request->pid, request->tamanio_proceso);

	t_memoria_inicializarproceso_response *response = inicializarproceso_response_new(numero_tablaprimernivel);
	int bytes;
	void *buffer_response = serializar_inicializarproceso_response(response, &bytes);
	enviar_por_socket(socket_cliente, buffer_response, bytes);

	free(buffer_response);
	inicializarproceso_response_destroy(response);
	inicializarproceso_request_destroy(request);
	free(buffer_request);
}

void suspender_proceso(int socket_cliente)
{
	enviar_string_con_longitud_por_socket(socket_cliente, "TEST: suspendiendo proceso...");
}

void finalizar_proceso(int socket_cliente)
{
	uint32_t bytes_request;
	recibir_uint32_por_socket(socket_cliente, &bytes_request);

	void *buffer_request = malloc(bytes_request);
	recibir_por_socket(socket_cliente, buffer_request, bytes_request);

	t_memoria_finalizarproceso_request *request = deserializar_finalizarproceso_request(buffer_request);

	log_debug(logger, "Finalizando proceso %d cuya tabla 1N es %d", request->pid, request->numero_tablaprimernivel);
	memoria_ram_finalizar_proceso(memoria_ram, request->numero_tablaprimernivel);

	t_memoria_finalizarproceso_response *response = finalizarproceso_response_new();
	int bytes;
	void *buffer_response = serializar_finalizarproceso_response(response, &bytes);
	enviar_por_socket(socket_cliente, buffer_response, bytes);

	free(buffer_response);
	finalizarproceso_response_destroy(response);
	finalizarproceso_request_destroy(request);
	free(buffer_request);
}

void handshake_info_traduccion(int socket_cliente)
{
	t_memoria_handshakeconfiguraciones_response *response = handshakeconfiguraciones_response_new(config->entradas_por_tabla, config->tamanio_pagina);
	int bytes;
	void *buffer_response = serializar_handshakeconfiguraciones_response(response, &bytes);
	enviar_por_socket(socket_cliente, buffer_response, bytes);

	free(buffer_response);
	handshakeconfiguraciones_response_destroy(response);
}

void leer_dato(int socket_cliente)
{
	uint32_t bytes_request;
	recibir_uint32_por_socket(socket_cliente, &bytes_request);

	void *buffer_request = malloc(bytes_request);
	recibir_por_socket(socket_cliente, buffer_request, bytes_request);

	t_memoria_leerdato_request *request = deserializar_leerdato_request(buffer_request);

	log_debug(logger, "Leyendo %d bytes de la direccion fisica %d", request->cantidad_bytes, request->direccion_fisica);
	void *dato = memoria_ram_leer_dato(memoria_ram, request);

	t_memoria_leerdato_response *response = leerdato_response_new(dato, request->cantidad_bytes);
	int bytes;
	void *buffer_response = serializar_leerdato_response(response, &bytes);
	enviar_por_socket(socket_cliente, buffer_response, bytes);

	free(buffer_response);
	leerdato_response_destroy(response);
	free(dato);
	leerdato_request_destroy(request);
	free(buffer_request);
}

void escribir_dato(int socket_cliente)
{
	uint32_t bytes_request;
	recibir_uint32_por_socket(socket_cliente, &bytes_request);

	void *buffer_request = malloc(bytes_request);
	recibir_por_socket(socket_cliente, buffer_request, bytes_request);

	t_memoria_escribirdato_request *request = deserializar_escribirdato_request(buffer_request);

	log_debug(logger, "Escribiendo %d bytes en la direccion fisica %d", request->cantidad_bytes, request->direccion_fisica);
	memoria_ram_escribir_dato(memoria_ram, request);

	t_memoria_escribirdato_response *response = escribirdato_response_new(request->cantidad_bytes);
	int bytes;
	void *buffer_response = serializar_escribirdato_response(response, &bytes);
	enviar_por_socket(socket_cliente, buffer_response, bytes);

	free(buffer_response);
	escribirdato_response_destroy(response);
	escribirdato_request_destroy(request);
	free(buffer_request);
}

void obtener_numero_tabla_2_para_entrada_tabla_1(int socket_cliente)
{
	uint32_t numero_tabla_1, entrada_tabla_1;
	recibir_uint32_por_socket(socket_cliente, &numero_tabla_1);
	recibir_uint32_por_socket(socket_cliente, &entrada_tabla_1);
	// TODO
}

void obtener_marco_para_entrada_tabla_2(int socket_cliente)
{
	uint32_t numero_tabla_1, numero_tabla_2, entrada_tabla_2;
	recibir_uint32_por_socket(socket_cliente, &numero_tabla_1);
	recibir_uint32_por_socket(socket_cliente, &numero_tabla_2);
	recibir_uint32_por_socket(socket_cliente, &entrada_tabla_2);
	//TODO
}
