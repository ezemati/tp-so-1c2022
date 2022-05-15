#include <memoria/numero_tabla_2_para_entrada_tabla_1.h>

t_memoria_numerotabla2paraentradatabla1_request *numerotabla2paraentradatabla1_request_new(uint32_t numero_tablaprimernivel, uint32_t entrada_tablaprimernivel)
{
    t_memoria_numerotabla2paraentradatabla1_request *request = malloc(sizeof(t_memoria_numerotabla2paraentradatabla1_request));
    request->numero_tablaprimernivel = numero_tablaprimernivel;
    request->entrada_tablaprimernivel = entrada_tablaprimernivel;
    return request;
}

void numerotabla2paraentradatabla1_request_destroy(t_memoria_numerotabla2paraentradatabla1_request *request)
{
    free(request);
}

t_memoria_numerotabla2paraentradatabla1_response *numerotabla2paraentradatabla1_response_new(uint32_t numero_tablasegundonivel)
{
    t_memoria_numerotabla2paraentradatabla1_response *response = malloc(sizeof(t_memoria_numerotabla2paraentradatabla1_response));
    response->numero_tablasegundonivel = numero_tablasegundonivel;
    return response;
}

void numerotabla2paraentradatabla1_response_destroy(t_memoria_numerotabla2paraentradatabla1_response *response)
{
    free(response);
}

/***************************** SERIALIZACION *****************************/

void *serializar_numerotabla2paraentradatabla1_request(t_memoria_numerotabla2paraentradatabla1_request *request, int *bytes)
{
    // TAM_BUFFER_REQUEST (uint32), NUMERO_PRIMERNIVEL (uint32), ENTRADA_PRIMERNIVEL (uint32)
    uint32_t bytes_buffer_sin_tamanio = bytes_totales_numerotabla2paraentradatabla1_request_serializada(request);
    (*bytes) = sizeof(uint32_t) + bytes_buffer_sin_tamanio;
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    escribir_uint32_en_buffer(buffer, &desplazamiento, bytes_buffer_sin_tamanio);

    escribir_uint32_en_buffer(buffer, &desplazamiento, request->numero_tablaprimernivel);
    escribir_uint32_en_buffer(buffer, &desplazamiento, request->entrada_tablaprimernivel);

    return buffer;
}

t_memoria_numerotabla2paraentradatabla1_request *deserializar_numerotabla2paraentradatabla1_request(void *buffer)
{
    int desplazamiento = 0;

    uint32_t numero_tablaprimernivel = leer_uint32_de_buffer(buffer, &desplazamiento);
    uint32_t entrada_tablaprimernivel = leer_uint32_de_buffer(buffer, &desplazamiento);

    t_memoria_numerotabla2paraentradatabla1_request *request = numerotabla2paraentradatabla1_request_new(numero_tablaprimernivel, entrada_tablaprimernivel);

    return request;
}

int bytes_totales_numerotabla2paraentradatabla1_request_serializada(t_memoria_numerotabla2paraentradatabla1_request *request)
{
    // NUMERO_PRIMERNIVEL (uint32), ENTRADA_PRIMERNIVEL (uint32)
    int bytes = 0;

    bytes += sizeof(request->numero_tablaprimernivel);
    bytes += sizeof(request->entrada_tablaprimernivel);

    return bytes;
}

void *serializar_numerotabla2paraentradatabla1_response(t_memoria_numerotabla2paraentradatabla1_response *response, int *bytes)
{
    // TAM_BUFFER_RESPONSE (uint32), NUMERO_TABLASEGUNDONIVEL (uint32)
    uint32_t bytes_buffer_sin_tamanio = bytes_totales_numerotabla2paraentradatabla1_response_serializada(response);
    (*bytes) = sizeof(uint32_t) + bytes_buffer_sin_tamanio;
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    escribir_uint32_en_buffer(buffer, &desplazamiento, bytes_buffer_sin_tamanio);

    escribir_uint32_en_buffer(buffer, &desplazamiento, response->numero_tablasegundonivel);

    return buffer;
}

t_memoria_numerotabla2paraentradatabla1_response *deserializar_numerotabla2paraentradatabla1_response(void *buffer)
{
    int desplazamiento = 0;

    uint32_t numero_tablasegundonivel = leer_uint32_de_buffer(buffer, &desplazamiento);

    t_memoria_numerotabla2paraentradatabla1_response *response = numerotabla2paraentradatabla1_response_new(numero_tablasegundonivel);

    return response;
}

int bytes_totales_numerotabla2paraentradatabla1_response_serializada(t_memoria_numerotabla2paraentradatabla1_response *response)
{
    // NUMERO_TABLASEGUNDONIVEL (uint32)
    int bytes = 0;

    bytes += sizeof(response->numero_tablasegundonivel);

    return bytes;
}
