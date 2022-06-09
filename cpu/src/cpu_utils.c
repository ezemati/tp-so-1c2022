#include <cpu_utils.h>

static void desalojar_proceso();
static void bloquear_proceso();
static void finalizar_proceso();
static void enviar_pcb_actualizado_a_kernel_con_instruccion(code_instruccion codigo_instruccion);
static t_kernel_actualizarpcb_request *crear_actualizarpcbrequest_para_infoejecucionactual();

void inicializar_cpu(int argc, char **argv)
{
	char *ruta_config = argc > 1
							? argv[1]
							: "cfg/cpu.config";
	config = cpu_config_new(ruta_config, logger);
	tlb = tlb_new(config->entradas_tlb);
}

void terminar_cpu()
{
	log_debug(logger, "Finalizando CPU...");
	log_destroy(logger);

	cpu_config_destroy(config);
	tlb_destroy(tlb);
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
	// log_debug(logger, "Cliente conectado por socket %d", socket_cliente);

	uint32_t id_op = -1;
	recibir_uint32_por_socket(socket_cliente, &id_op);

	log_info_if_logger_not_null(logger, "Operacion recibida en CPU: %s", identificador_operacion_to_string(id_op));

	switch (id_op)
	{
	case HANDSHAKE_SOY_KERNEL:
		atender_handshake_con_kernel(socket_cliente);
		break;
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

	log_info_if_logger_not_null(logger, "Handshake finalizado - {entradas_por_tabla=%d, tamanio_pagina=%d}", config->memoria_entradas_por_tabla, config->memoria_tamanio_pagina);

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
			uint32_t direccion_logica_origen = instruccion_a_ejecutar->parametros[1];
			bool direccion_valida = direccion_logica_valida(direccion_logica_origen);
			if (!direccion_valida)
			{
				// TODO: ver si aca desalojamos al proceso por intentar acceder a una direccion no valida
				log_error_if_logger_not_null(logger, "Direccion %d invalida (el tamanio del proceso es %d)", direccion_logica_origen, info_ejecucion_actual->tamanio);
			}
			else
			{
				valor_para_copy = fetch_valor_para_copy(direccion_logica_origen);
			}
		}

		// EXECUTE
		ejecutar_instruccion(instruccion_a_ejecutar, valor_para_copy);

		// Incrementar el Program Counter
		info_ejecucion_actual->program_counter++;

		info_ejecucion_actual->ultima_instruccion_ejecutada = instruccion_a_ejecutar->codigo_instruccion;

		// CHECK INTERRUPT (se chequea automaticamente por la condicion del while)
	} while (!hay_interrupcion && info_ejecucion_actual->ultima_instruccion_ejecutada != IO && !ejecucion_completada(info_ejecucion_actual) && info_ejecucion_actual->ultima_instruccion_ejecutada != EXIT);

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

uint32_t leer_dato(uint32_t direccion_logica_lectura)
{
	uint32_t numero_tablasegundonivel, entrada_tablasegundonivel;
	uint32_t direccion_fisica_lectura = traducir_direccion_logica_a_fisica(direccion_logica_lectura, &numero_tablasegundonivel, &entrada_tablasegundonivel);

	int socket_memoria = crear_conexion(config->ip_memoria, config->puerto_memoria, NULL);

	// ? Ver si implementamos lo de leer de multiples paginas
	uint32_t cantidad_bytes_lectura = sizeof(uint32_t);
	t_memoria_leerdato_request *request = leerdato_request_new(numero_tablasegundonivel, entrada_tablasegundonivel, direccion_fisica_lectura, cantidad_bytes_lectura);
	int bytes_request_serializada;
	void *request_serializada = serializar_leerdato_request(request, &bytes_request_serializada);
	enviar_buffer_serializado_con_instruccion_y_bytes_por_socket(socket_memoria, LEER_DATO, request_serializada, bytes_request_serializada);
	free(request_serializada);
	leerdato_request_destroy(request);

	void *response_serializada = NULL;
	recibir_buffer_con_bytes_por_socket(socket_memoria, &response_serializada);
	t_memoria_leerdato_response *response = deserializar_leerdato_response(response_serializada);
	uint32_t dato_leido = *((uint32_t *)response->dato);
	leerdato_response_destroy(response);
	free(response_serializada);

	return dato_leido;
}

void escribir_o_copiar_dato(uint32_t direccion_logica_destino, uint32_t valor_a_escribir_o_copiar)
{
	uint32_t numero_tablasegundonivel, entrada_tablasegundonivel;
	uint32_t direccion_fisica_escritura = traducir_direccion_logica_a_fisica(direccion_logica_destino, &numero_tablasegundonivel, &entrada_tablasegundonivel);

	int socket_memoria = crear_conexion(config->ip_memoria, config->puerto_memoria, NULL);

	// ? Ver si implementamos lo de escribir en multiples paginas
	uint32_t cantidad_bytes_escritura = sizeof(uint32_t);
	t_memoria_escribirdato_request *request = escribirdato_request_new(numero_tablasegundonivel, entrada_tablasegundonivel, direccion_fisica_escritura, cantidad_bytes_escritura, &valor_a_escribir_o_copiar);
	int bytes_request_serializada;
	void *request_serializada = serializar_escribirdato_request(request, &bytes_request_serializada);
	enviar_buffer_serializado_con_instruccion_y_bytes_por_socket(socket_memoria, ESCRIBIR_DATO, request_serializada, bytes_request_serializada);
	free(request_serializada);
	escribirdato_request_destroy(request);

	void *response_serializada = NULL;
	recibir_buffer_con_bytes_por_socket(socket_memoria, &response_serializada);
	t_memoria_escribirdato_response *response = deserializar_escribirdato_response(response_serializada);
	// uint32_t cantidad_bytes_escritos = response->cantidad_bytes;
	escribirdato_response_destroy(response);
	free(response_serializada);
}

void proceso_desalojado_de_cpu()
{
	double milisegundos_en_running = milisegundos_entre_times(info_ejecucion_actual->time_inicio_running, info_ejecucion_actual->time_fin_running);
	log_trace_if_logger_not_null(logger, "Desalojando proceso %d (ejecuto %fms)", info_ejecucion_actual->pid, milisegundos_en_running);

	tlb_clear(tlb);
	infoejecucionactual_destroy(info_ejecucion_actual);
}

static void desalojar_proceso()
{
	t_kernel_actualizarpcb_request *request = crear_actualizarpcbrequest_para_infoejecucionactual();
	int bytes_request_serializada;
	void *request_serializada = serializar_actualizarpcb_request(request, &bytes_request_serializada);
	enviar_buffer_serializado_con_bytes_por_socket(socket_conexion_kernel_interrupt, request_serializada, bytes_request_serializada);

	liberar_conexion(socket_conexion_kernel_interrupt);
	free(request_serializada);
	actualizarpcb_request_destroy(request);

	proceso_desalojado_de_cpu();
}

static void bloquear_proceso()
{
	enviar_pcb_actualizado_a_kernel_con_instruccion(BLOQUEAR_PROCESO);

	proceso_desalojado_de_cpu();
}

static void finalizar_proceso()
{
	enviar_pcb_actualizado_a_kernel_con_instruccion(FINALIZAR_PROCESO);

	proceso_desalojado_de_cpu();
}

static void enviar_pcb_actualizado_a_kernel_con_instruccion(code_instruccion codigo_instruccion)
{
	t_kernel_actualizarpcb_request *request = crear_actualizarpcbrequest_para_infoejecucionactual();
	int bytes_request_serializada;
	void *request_serializada = serializar_actualizarpcb_request(request, &bytes_request_serializada);
	enviar_buffer_serializado_con_instruccion_y_bytes_por_socket(socket_conexion_kernel_dispatch, codigo_instruccion, request_serializada, bytes_request_serializada);
	free(request_serializada);
	actualizarpcb_request_destroy(request);
}

static t_kernel_actualizarpcb_request *crear_actualizarpcbrequest_para_infoejecucionactual()
{
	t_kernel_actualizarpcb_request *request = actualizarpcb_request_new(info_ejecucion_actual->pid, info_ejecucion_actual->program_counter, info_ejecucion_actual->bloqueo_pendiente, info_ejecucion_actual->time_inicio_running, info_ejecucion_actual->time_fin_running);
	return request;
}
