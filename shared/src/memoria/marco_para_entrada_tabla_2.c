#include <memoria/marco_para_entrada_tabla_2.h>

t_memoria_marcoparaentradatabla2_request *marcoparaentradatabla2_request_new(uint32_t numero_tablasegundonivel, uint32_t entrada_tablasegundonivel)
{
    t_memoria_marcoparaentradatabla2_request *request = malloc(sizeof(t_memoria_marcoparaentradatabla2_request));
    request->numero_tablasegundonivel = numero_tablasegundonivel;
    request->entrada_tablasegundonivel = entrada_tablasegundonivel;
    return request;
}

void marcoparaentradatabla2_request_destroy(t_memoria_marcoparaentradatabla2_request *request)
{
    free(request);
}

t_memoria_marcoparaentradatabla2_response *marcoparaentradatabla2_response_new(uint32_t numero_marco)
{
    t_memoria_marcoparaentradatabla2_response *response = malloc(sizeof(t_memoria_marcoparaentradatabla2_response));
    response->numero_marco = numero_marco;
    return response;
}

void marcoparaentradatabla2_response_destroy(t_memoria_marcoparaentradatabla2_response *response)
{
    free(response);
}

/***************************** SERIALIZACION *****************************/

void *serializar_marcoparaentradatabla2_request(t_memoria_marcoparaentradatabla2_request *request, int *bytes)
{
    // TAM_BUFFER_REQUEST (uint32), NUMERO_SEGUNDONIVEL (uint32), ENTRADA_SEGUNDONIVEL (uint32)
    uint32_t bytes_buffer_sin_tamanio = bytes_totales_marcoparaentradatabla2_request_serializada(request);
    (*bytes) = sizeof(uint32_t) + bytes_buffer_sin_tamanio;
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    escribir_uint32(buffer, &desplazamiento, bytes_buffer_sin_tamanio);

    escribir_uint32(buffer, &desplazamiento, request->numero_tablasegundonivel);
    escribir_uint32(buffer, &desplazamiento, request->entrada_tablasegundonivel);

    return buffer;
}

t_memoria_marcoparaentradatabla2_request *deserializar_marcoparaentradatabla2_request(void *buffer)
{
    int desplazamiento = 0;

    uint32_t numero_tablasegundonivel = leer_uint32(buffer, &desplazamiento);
    uint32_t entrada_tablasegundonivel = leer_uint32(buffer, &desplazamiento);

    t_memoria_marcoparaentradatabla2_request *request = marcoparaentradatabla2_request_new(numero_tablasegundonivel, entrada_tablasegundonivel);

    return request;
}

int bytes_totales_marcoparaentradatabla2_request_serializada(t_memoria_marcoparaentradatabla2_request *request)
{
    // NUMERO_SEGUNDONIVEL (uint32), ENTRADA_SEGUNDONIVEL (uint32)
    int bytes = 0;

    bytes += sizeof(request->numero_tablasegundonivel);
    bytes += sizeof(request->entrada_tablasegundonivel);

    return bytes;
}

void *serializar_marcoparaentradatabla2_response(t_memoria_marcoparaentradatabla2_response *response, int *bytes)
{
    // TAM_BUFFER_RESPONSE (uint32), NUMERO_MARCO (uint32)
    uint32_t bytes_buffer_sin_tamanio = bytes_totales_marcoparaentradatabla2_response_serializada(response);
    (*bytes) = sizeof(uint32_t) + bytes_buffer_sin_tamanio;
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    escribir_uint32(buffer, &desplazamiento, bytes_buffer_sin_tamanio);

    escribir_uint32(buffer, &desplazamiento, response->numero_marco);

    return buffer;
}

t_memoria_marcoparaentradatabla2_response *deserializar_marcoparaentradatabla2_response(void *buffer)
{
    int desplazamiento = 0;

    uint32_t numero_marco = leer_uint32(buffer, &desplazamiento);

    t_memoria_marcoparaentradatabla2_response *response = marcoparaentradatabla2_response_new(numero_marco);

    return response;
}

int bytes_totales_marcoparaentradatabla2_response_serializada(t_memoria_marcoparaentradatabla2_response *response)
{
    // NUMERO_MARCO (uint32)
    int bytes = 0;

    bytes += sizeof(response->numero_marco);

    return bytes;
}
