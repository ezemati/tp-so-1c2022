#include <cpu_mmu.h>

static uint32_t obtener_numero_tabla_2_para_entrada_tabla_1(uint32_t numero_tablaprimernivel, uint32_t entrada_tablaprimernivel);
static uint32_t obtener_numero_marco_para_entrada_tabla_2(uint32_t numero_tablaprimernivel, uint32_t numero_tablasegundonivel, uint32_t entrada_tablasegundonivel);

bool direccion_logica_valida(uint32_t direccion_logica)
{
    // Si el tamanio del proceso es 30, entonces puede direccionar desde el byte 0 al 29
    return direccion_logica < info_ejecucion_actual->tamanio;
}

uint32_t traducir_direccion_logica_a_fisica(uint32_t direccion_logica)
{
    uint32_t numero_pagina = floor(direccion_logica / config->memoria_tamanio_pagina);

    log_info_if_logger_not_null(logger, "MMU: traduciendo direccion logica %d (numero_pagina=%d)", direccion_logica, numero_pagina);

    int numero_marco = tlb_try_read_entry(tlb, numero_pagina);
    if (numero_marco != -1)
    {
        // Si encontre el numero_pagina en la TLB, directamente uso el numero_marco
        log_trace_if_logger_not_null(logger, "TLB HIT: {numero_pagina=%d, numero_marco=%d}", numero_pagina, numero_marco);
    }
    else
    {
        // Si no encontre el numero_pagina en la TLB, hago todo el procedimiento para obtener el numero_marco
        // y al final agrego la entrada a la TLB
        log_trace_if_logger_not_null(logger, "TLB MISS: la pagina %d no esta en la TLB", numero_pagina);

        uint32_t entrada_tablaprimernivel = floor(numero_pagina / config->memoria_entradas_por_tabla);
        uint32_t entrada_tablasegundonivel = numero_pagina % config->memoria_entradas_por_tabla;

        uint32_t numero_tablasegundonivel = obtener_numero_tabla_2_para_entrada_tabla_1(info_ejecucion_actual->tabla_paginas_primer_nivel, entrada_tablaprimernivel);
        numero_marco = obtener_numero_marco_para_entrada_tabla_2(info_ejecucion_actual->tabla_paginas_primer_nivel, numero_tablasegundonivel, entrada_tablasegundonivel);

        tlb_add_entry(tlb, numero_pagina, numero_marco);
        log_trace_if_logger_not_null(logger, "TLB: entrada {numero_pagina=%d, numero_marco=%d} agregada", numero_pagina, numero_marco);
    }

    uint32_t desplazamiento = direccion_logica - (numero_pagina * config->memoria_tamanio_pagina);
    uint32_t direccion_fisica = (numero_marco * config->memoria_tamanio_pagina) + desplazamiento;
    log_info_if_logger_not_null(logger, "MMU: {direccion_logica=%d, direccion_fisica=%d}", direccion_logica, direccion_fisica);

    return direccion_fisica;
}

uint32_t fetch_valor_para_copy(uint32_t direccion_logica_origen)
{
    return leer_dato(direccion_logica_origen);
}

static uint32_t obtener_numero_tabla_2_para_entrada_tabla_1(uint32_t numero_tablaprimernivel, uint32_t entrada_tablaprimernivel)
{
    int socket_memoria = crear_conexion(config->ip_memoria, config->puerto_memoria, NULL);

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

    liberar_conexion(socket_memoria);

    return numero_tablasegundonivel;
}

static uint32_t obtener_numero_marco_para_entrada_tabla_2(uint32_t numero_tablaprimernivel, uint32_t numero_tablasegundonivel, uint32_t entrada_tablasegundonivel)
{
    int socket_memoria = crear_conexion(config->ip_memoria, config->puerto_memoria, NULL);

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

    liberar_conexion(socket_memoria);

    return numero_marco;
}
