#include <cpu_utils.h>

static uint32_t obtener_numero_tabla_2_para_entrada_tabla_1(int socket_memoria, uint32_t numero_tablaprimernivel, uint32_t entrada_tablaprimernivel);
static uint32_t obtener_numero_marco_para_entrada_tabla_2(int socket_memoria, uint32_t numero_tablaprimernivel, uint32_t numero_tablasegundonivel, uint32_t entrada_tablasegundonivel);
static uint32_t fetch_valor_para_copy(uint32_t direccion_logica_origen);
static void desalojar_proceso();
static void bloquear_proceso();
static void finalizar_proceso();

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
	case HANDSHAKE_SOY_KERNEL:
		atender_handshake_con_kernel(socket_cliente);
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

void realizar_ejecucion()
{
	// Ejecuta infinitamente hasta que llegue una interrupcion del Kernel,
	// ejecute una instruccion de IO o finalice (instruccion EXIT)
	info_ejecucion_actual->time_inicio_running = current_time();
	do
	{
		// FETCH
		t_instruccion *instruccion_a_ejecutar = list_get(info_ejecucion_actual->lista_instrucciones, info_ejecucion_actual->program_counter);

		// DECODE y FETCH OPERANDS
		uint32_t valor_para_copy = 0;
		if (instruccion_a_ejecutar->codigo_instruccion == COPY)
		{
			valor_para_copy = fetch_valor_para_copy(instruccion_a_ejecutar->parametros[1]);
		}

		// EXECUTE
		ejecutar_instruccion(instruccion_a_ejecutar, valor_para_copy);

		// Incrementar el Program Counter
		info_ejecucion_actual->program_counter++;

		info_ejecucion_actual->ultima_instruccion_ejecutada = instruccion_a_ejecutar->codigo_instruccion;

		// CHECK INTERRUPT (se chequea automaticamente por la condicion del while)
	} while (!ejecucion_completada(info_ejecucion_actual) && !hay_interrupcion && info_ejecucion_actual->ultima_instruccion_ejecutada != IO && info_ejecucion_actual->ultima_instruccion_ejecutada != EXIT);

	info_ejecucion_actual->time_fin_running = current_time();

	if (hay_interrupcion)
	{
		desalojar_proceso();
	}
	else if (info_ejecucion_actual->ultima_instruccion_ejecutada == IO)
	{
		bloquear_proceso();
	}
	else if (ejecucion_completada(info_ejecucion_actual) || info_ejecucion_actual->ultima_instruccion_ejecutada == EXIT)
	{
		finalizar_proceso();
	}
	else
	{
		log_error_if_logger_not_null(logger, "Aca paso algo raro...");
	}
}

uint32_t traducir_direccion_logica_a_fisica(uint32_t direccion_logica)
{
	uint32_t numero_pagina = floor(direccion_logica / config->memoria_tamanio_pagina);
	uint32_t entrada_tablaprimernivel = floor(numero_pagina / config->memoria_entradas_por_tabla);
	uint32_t entrada_tablasegundonivel = numero_pagina % config->memoria_entradas_por_tabla;
	uint32_t desplazamiento = direccion_logica - (numero_pagina * config->memoria_tamanio_pagina);

	int socket_memoria = crear_conexion(config->ip_memoria, config->puerto_memoria, logger);

	uint32_t numero_tablasegundonivel = obtener_numero_tabla_2_para_entrada_tabla_1(socket_memoria, info_ejecucion_actual->tabla_paginas_primer_nivel, entrada_tablaprimernivel);
	uint32_t numero_marco = obtener_numero_marco_para_entrada_tabla_2(socket_memoria, info_ejecucion_actual->tabla_paginas_primer_nivel, numero_tablasegundonivel, entrada_tablasegundonivel);

	uint32_t direccion_fisica = (numero_marco * config->memoria_tamanio_pagina) + desplazamiento;

	liberar_conexion(socket_memoria);

	log_info_if_logger_not_null(logger, "Traduccion -- Logica=%d -- Fisica=%d", direccion_logica, direccion_fisica);

	return direccion_fisica;
}

static uint32_t obtener_numero_tabla_2_para_entrada_tabla_1(int socket_memoria, uint32_t numero_tablaprimernivel, uint32_t entrada_tablaprimernivel)
{
	t_memoria_numerotabla2paraentradatabla1_request *request_numerotabla2paraentradatabla1 = numerotabla2paraentradatabla1_request_new(info_ejecucion_actual->tabla_paginas_primer_nivel, entrada_tablaprimernivel);
	int bytes_request_numerotabla2paraentradatabla1_serializada;
	void *request_numerotabla2paraentradatabla1_serializada = serializar_numerotabla2paraentradatabla1_request(request_numerotabla2paraentradatabla1, &bytes_request_numerotabla2paraentradatabla1_serializada);
	enviar_buffer_serializado_con_instruccion_y_bytes_por_socket(socket_memoria, OBTENER_NUMERO_TABLA_2_PARA_ENTRADA_TABLA_1, request_numerotabla2paraentradatabla1_serializada, bytes_request_numerotabla2paraentradatabla1_serializada);
	free(request_numerotabla2paraentradatabla1_serializada);
	numerotabla2paraentradatabla1_request_destroy(request_numerotabla2paraentradatabla1);

	void *response_numerotabla2paraentradatabla1_serializada = NULL;
	recibir_buffer_con_bytes_por_socket(socket_memoria, &response_numerotabla2paraentradatabla1_serializada);
	t_memoria_numerotabla2paraentradatabla1_response *response_numerotabla2paraentradatabla1 = deserializar_numerotabla2paraentradatabla1_response(response_numerotabla2paraentradatabla1_serializada);
	uint32_t numero_tablasegundonivel = response_numerotabla2paraentradatabla1->numero_tablasegundonivel;
	numerotabla2paraentradatabla1_response_destroy(response_numerotabla2paraentradatabla1);
	free(response_numerotabla2paraentradatabla1_serializada);

	return numero_tablasegundonivel;
}

static uint32_t obtener_numero_marco_para_entrada_tabla_2(int socket_memoria, uint32_t numero_tablaprimernivel, uint32_t numero_tablasegundonivel, uint32_t entrada_tablasegundonivel)
{
	t_memoria_marcoparaentradatabla2_request *request_marcoparaentradatabla2 = marcoparaentradatabla2_request_new(numero_tablaprimernivel, numero_tablasegundonivel, entrada_tablasegundonivel);
	int bytes_request_marcoparaentradatabla2_serializada;
	void *request_marcoparaentradatabla2_serializada = serializar_marcoparaentradatabla2_request(request_marcoparaentradatabla2, &bytes_request_marcoparaentradatabla2_serializada);
	enviar_buffer_serializado_con_instruccion_y_bytes_por_socket(socket_memoria, OBTENER_MARCO_PARA_ENTRADA_TABLA_2, request_marcoparaentradatabla2_serializada, bytes_request_marcoparaentradatabla2_serializada);
	free(request_marcoparaentradatabla2_serializada);
	marcoparaentradatabla2_request_destroy(request_marcoparaentradatabla2);

	void *response_marcoparaentradatabla2_serializada = NULL;
	recibir_buffer_con_bytes_por_socket(socket_memoria, &response_marcoparaentradatabla2_serializada);
	t_memoria_marcoparaentradatabla2_response *response_marcoparaentradatabla2 = deserializar_marcoparaentradatabla2_response(response_marcoparaentradatabla2_serializada);
	uint32_t numero_marco = response_marcoparaentradatabla2->numero_marco;
	marcoparaentradatabla2_response_destroy(response_marcoparaentradatabla2);
	free(response_marcoparaentradatabla2_serializada);

	return numero_marco;
}

static uint32_t fetch_valor_para_copy(uint32_t direccion_logica_origen)
{
	return 0;
}

static void desalojar_proceso()
{
}

static void bloquear_proceso()
{
}

static void finalizar_proceso()
{
}
