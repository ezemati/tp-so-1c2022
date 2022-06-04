#include <memoria_requests.h>

void inicializar_proceso(int socket_cliente)
{
	void *buffer_request = NULL;
	recibir_buffer_con_bytes_por_socket(socket_cliente, &buffer_request);

	t_memoria_inicializarproceso_request *request = deserializar_inicializarproceso_request(buffer_request);

	log_debug(logger, "Inicializando estructuras para proceso %d con tamanio %d", request->pid, request->tamanio_proceso);
	uint32_t numero_tablaprimernivel = memoria_ram_agregar_proceso(memoria_ram, request->pid, request->tamanio_proceso);

	t_memoria_inicializarproceso_response *response = inicializarproceso_response_new(numero_tablaprimernivel);
	int bytes;
	void *buffer_response = serializar_inicializarproceso_response(response, &bytes);
	enviar_buffer_serializado_con_bytes_por_socket(socket_cliente, buffer_response, bytes);

	free(buffer_response);
	inicializarproceso_response_destroy(response);
	inicializarproceso_request_destroy(request);
	free(buffer_request);
}

void suspender_proceso(int socket_cliente)
{
	// TODO
	void *buffer_request = NULL;
	recibir_buffer_con_bytes_por_socket(socket_cliente, &buffer_request);

	t_memoria_suspenderproceso_request *request = deserializar_suspenderproceso_request(buffer_request);

	log_debug(logger, "Suspendiendo proceso %d cuya tabla 1N es %d", request->pid, request->numero_tablaprimernivel);
	// memoria_ram_suspender_proceso(memoria_ram, request->numero_tablaprimernivel);

	bool ok = true;
	t_memoria_suspenderproceso_response *response = suspenderproceso_response_new(ok);
	int bytes;
	void *buffer_response = serializar_suspenderproceso_response(response, &bytes);
	enviar_buffer_serializado_con_bytes_por_socket(socket_cliente, buffer_response, bytes);

	free(buffer_response);
	suspenderproceso_response_destroy(response);
	suspenderproceso_request_destroy(request);
	free(buffer_request);
}

void finalizar_proceso(int socket_cliente)
{
	void *buffer_request = NULL;
	recibir_buffer_con_bytes_por_socket(socket_cliente, &buffer_request);

	t_memoria_finalizarproceso_request *request = deserializar_finalizarproceso_request(buffer_request);

	log_debug(logger, "Finalizando proceso %d cuya tabla 1N es %d", request->pid, request->numero_tablaprimernivel);
	memoria_ram_finalizar_proceso(memoria_ram, request->numero_tablaprimernivel);

	t_memoria_finalizarproceso_response *response = finalizarproceso_response_new();
	int bytes;
	void *buffer_response = serializar_finalizarproceso_response(response, &bytes);
	enviar_buffer_serializado_con_bytes_por_socket(socket_cliente, buffer_response, bytes);

	free(buffer_response);
	finalizarproceso_response_destroy(response);
	finalizarproceso_request_destroy(request);
	free(buffer_request);
}

void handshake_info_traduccion(int socket_cliente)
{
	t_memoria_handshakeconfiguraciones_response *response = handshakeconfiguraciones_memoria_response_new(config->entradas_por_tabla, config->tamanio_pagina);
	int bytes;
	void *buffer_response = serializar_handshakeconfiguraciones_memoria_response(response, &bytes);
	enviar_buffer_serializado_con_bytes_por_socket(socket_cliente, buffer_response, bytes);

	free(buffer_response);
	handshakeconfiguraciones_memoria_response_destroy(response);
}

void leer_dato(int socket_cliente)
{
	void *buffer_request = NULL;
	recibir_buffer_con_bytes_por_socket(socket_cliente, &buffer_request);

	t_memoria_leerdato_request *request = deserializar_leerdato_request(buffer_request);

	log_debug(logger, "Leyendo %d bytes de la direccion fisica %d", request->cantidad_bytes, request->direccion_fisica);
	void *dato = memoria_ram_leer_dato(memoria_ram, request);

	t_memoria_leerdato_response *response = leerdato_response_new(dato, request->cantidad_bytes);
	int bytes;
	void *buffer_response = serializar_leerdato_response(response, &bytes);
	enviar_buffer_serializado_con_bytes_por_socket(socket_cliente, buffer_response, bytes);

	free(buffer_response);
	leerdato_response_destroy(response);
	free(dato);
	leerdato_request_destroy(request);
	free(buffer_request);
}

void escribir_dato(int socket_cliente)
{
	void *buffer_request = NULL;
	recibir_buffer_con_bytes_por_socket(socket_cliente, &buffer_request);

	t_memoria_escribirdato_request *request = deserializar_escribirdato_request(buffer_request);

	log_debug(logger, "Escribiendo %d bytes en la direccion fisica %d", request->cantidad_bytes, request->direccion_fisica);
	memoria_ram_escribir_dato(memoria_ram, request);

	t_memoria_escribirdato_response *response = escribirdato_response_new(request->cantidad_bytes);
	int bytes;
	void *buffer_response = serializar_escribirdato_response(response, &bytes);
	enviar_buffer_serializado_con_bytes_por_socket(socket_cliente, buffer_response, bytes);

	free(buffer_response);
	escribirdato_response_destroy(response);
	escribirdato_request_destroy(request);
	free(buffer_request);
}

void obtener_numero_tabla_2_para_entrada_tabla_1(int socket_cliente)
{
	void *buffer_request = NULL;
	recibir_buffer_con_bytes_por_socket(socket_cliente, &buffer_request);

	t_memoria_numerotabla2paraentradatabla1_request *request = deserializar_numerotabla2paraentradatabla1_request(buffer_request);

	log_debug(logger, "Buscando numero de tabla 2 para entrada %d de la tabla de primer nivel %d", request->entrada_tablaprimernivel, request->numero_tablaprimernivel);
	uint32_t numero_tablasegundonivel = memoria_ram_obtener_numero_tabla_2_para_entrada_tabla_1(memoria_ram, request);

	t_memoria_numerotabla2paraentradatabla1_response *response = numerotabla2paraentradatabla1_response_new(numero_tablasegundonivel);
	int bytes;
	void *buffer_response = serializar_numerotabla2paraentradatabla1_response(response, &bytes);
	enviar_buffer_serializado_con_bytes_por_socket(socket_cliente, buffer_response, bytes);

	free(buffer_response);
	numerotabla2paraentradatabla1_response_destroy(response);
	numerotabla2paraentradatabla1_request_destroy(request);
	free(buffer_request);
}

void obtener_marco_para_entrada_tabla_2(int socket_cliente)
{
	void *buffer_request = NULL;
	recibir_buffer_con_bytes_por_socket(socket_cliente, &buffer_request);

	t_memoria_marcoparaentradatabla2_request *request = deserializar_marcoparaentradatabla2_request(buffer_request);

	log_debug(logger, "Buscando numero de marco para entrada %d de la tabla de segundo nivel %d", request->entrada_tablasegundonivel, request->numero_tablasegundonivel);
	uint32_t numero_marco = memoria_ram_obtener_numero_marco_para_entrada_tabla_2(memoria_ram, request);

	t_memoria_marcoparaentradatabla2_response *response = marcoparaentradatabla2_response_new(numero_marco);
	int bytes;
	void *buffer_response = serializar_marcoparaentradatabla2_response(response, &bytes);
	enviar_buffer_serializado_con_bytes_por_socket(socket_cliente, buffer_response, bytes);

	free(buffer_response);
	marcoparaentradatabla2_response_destroy(response);
	marcoparaentradatabla2_request_destroy(request);
	free(buffer_request);
}
