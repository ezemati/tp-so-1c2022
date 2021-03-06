#include <memoria/marco_para_entrada_tabla_2.h>

t_memoria_marcoparaentradatabla2_request *marcoparaentradatabla2_request_new(uint32_t numero_tablaprimernivel, uint32_t numero_tablasegundonivel, uint32_t entrada_tablasegundonivel)
{
    t_memoria_marcoparaentradatabla2_request *request = malloc(sizeof(t_memoria_marcoparaentradatabla2_request));
    request->numero_tablaprimernivel = numero_tablaprimernivel;
    request->numero_tablasegundonivel = numero_tablasegundonivel;
    request->entrada_tablasegundonivel = entrada_tablasegundonivel;
    return request;
}

void marcoparaentradatabla2_request_destroy(t_memoria_marcoparaentradatabla2_request *request)
{
    free(request);
}

t_memoria_marcoparaentradatabla2_response *marcoparaentradatabla2_response_new(uint32_t numero_marco, int32_t numero_pagina_reemplazada)
{
    t_memoria_marcoparaentradatabla2_response *response = malloc(sizeof(t_memoria_marcoparaentradatabla2_response));
    response->numero_marco = numero_marco;
    response->numero_pagina_reemplazada = numero_pagina_reemplazada;
    return response;
}

void marcoparaentradatabla2_response_destroy(t_memoria_marcoparaentradatabla2_response *response)
{
    free(response);
}

/***************************** SERIALIZACION *****************************/

void *serializar_marcoparaentradatabla2_request(t_memoria_marcoparaentradatabla2_request *request, int *bytes)
{
    // NUMERO_PRIMERNIVEL (uint32), NUMERO_SEGUNDONIVEL (uint32), ENTRADA_SEGUNDONIVEL (uint32)
    (*bytes) = bytes_totales_marcoparaentradatabla2_request_serializada(request);
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    escribir_uint32_en_buffer(buffer, &desplazamiento, request->numero_tablaprimernivel);
    escribir_uint32_en_buffer(buffer, &desplazamiento, request->numero_tablasegundonivel);
    escribir_uint32_en_buffer(buffer, &desplazamiento, request->entrada_tablasegundonivel);

    return buffer;
}

t_memoria_marcoparaentradatabla2_request *deserializar_marcoparaentradatabla2_request(void *buffer)
{
    int desplazamiento = 0;

    uint32_t numero_tablaprimernivel = leer_uint32_de_buffer(buffer, &desplazamiento);
    uint32_t numero_tablasegundonivel = leer_uint32_de_buffer(buffer, &desplazamiento);
    uint32_t entrada_tablasegundonivel = leer_uint32_de_buffer(buffer, &desplazamiento);

    t_memoria_marcoparaentradatabla2_request *request = marcoparaentradatabla2_request_new(numero_tablaprimernivel, numero_tablasegundonivel, entrada_tablasegundonivel);

    return request;
}

int bytes_totales_marcoparaentradatabla2_request_serializada(t_memoria_marcoparaentradatabla2_request *request)
{
    // NUMERO_PRIMERNIVEL (uint32), NUMERO_SEGUNDONIVEL (uint32), ENTRADA_SEGUNDONIVEL (uint32)
    int bytes = 0;

    bytes += sizeof(request->numero_tablaprimernivel);
    bytes += sizeof(request->numero_tablasegundonivel);
    bytes += sizeof(request->entrada_tablasegundonivel);

    return bytes;
}

void *serializar_marcoparaentradatabla2_response(t_memoria_marcoparaentradatabla2_response *response, int *bytes)
{
    // NUMERO_MARCO (uint32), NUMERO_PAGINA_REEMPLAZADA (int32)
    (*bytes) = bytes_totales_marcoparaentradatabla2_response_serializada(response);
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    escribir_uint32_en_buffer(buffer, &desplazamiento, response->numero_marco);
    escribir_int32_en_buffer(buffer, &desplazamiento, response->numero_pagina_reemplazada);

    return buffer;
}

t_memoria_marcoparaentradatabla2_response *deserializar_marcoparaentradatabla2_response(void *buffer)
{
    int desplazamiento = 0;

    uint32_t numero_marco = leer_uint32_de_buffer(buffer, &desplazamiento);
    int32_t numero_pagina_reemplazada = leer_int32_de_buffer(buffer, &desplazamiento);

    t_memoria_marcoparaentradatabla2_response *response = marcoparaentradatabla2_response_new(numero_marco, numero_pagina_reemplazada);

    return response;
}

int bytes_totales_marcoparaentradatabla2_response_serializada(t_memoria_marcoparaentradatabla2_response *response)
{
    // NUMERO_MARCO (uint32), NUMERO_PAGINA_REEMPLAZADA (int32)
    int bytes = 0;

    bytes += sizeof(response->numero_marco);
    bytes += sizeof(response->numero_pagina_reemplazada);

    return bytes;
}
