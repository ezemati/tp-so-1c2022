#include <memoria/leer_dato.h>

t_memoria_leerdato_request *leerdato_request_new(uint32_t direccion_fisica, uint32_t cantidad_bytes)
{
    t_memoria_leerdato_request *request = malloc(sizeof(t_memoria_leerdato_request));
    request->direccion_fisica = direccion_fisica;
    request->cantidad_bytes = cantidad_bytes;
    return request;
}

void leerdato_request_destroy(t_memoria_leerdato_request *request)
{
    free(request);
}

t_memoria_leerdato_response *leerdato_response_new(void *dato, uint32_t cantidad_bytes)
{
    t_memoria_leerdato_response *response = malloc(sizeof(t_memoria_leerdato_response));

    response->cantidad_bytes = cantidad_bytes;

    response->dato = malloc(response->cantidad_bytes);
    memcpy(response->dato, dato, response->cantidad_bytes);

    return response;
}

void leerdato_response_destroy(t_memoria_leerdato_response *response)
{
    free(response->dato);
    free(response);
}

/***************************** SERIALIZACION *****************************/

void *serializar_leerdato_request(t_memoria_leerdato_request *request, int *bytes)
{
    // TAM_BUFFER_REQUEST (uint32), DIR_FISICA (uint32), CANT_BYTES (uint32)
    uint32_t bytes_buffer_sin_tamanio = bytes_totales_leerdato_request_serializada(request);
    (*bytes) = sizeof(uint32_t) + bytes_buffer_sin_tamanio;
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    escribir_uint32(buffer, &desplazamiento, bytes_buffer_sin_tamanio);

    escribir_uint32(buffer, &desplazamiento, request->direccion_fisica);
    escribir_uint32(buffer, &desplazamiento, request->cantidad_bytes);

    return buffer;
}

t_memoria_leerdato_request *deserializar_leerdato_request(void *buffer)
{
    int desplazamiento = 0;

    uint32_t direccion_fisica = leer_uint32(buffer, &desplazamiento);
    uint32_t cantidad_bytes = leer_uint32(buffer, &desplazamiento);

    t_memoria_leerdato_request *request = leerdato_request_new(direccion_fisica, cantidad_bytes);
    return request;
}

int bytes_totales_leerdato_request_serializada(t_memoria_leerdato_request *request)
{
    // DIR_FISICA (uint32), CANT_BYTES (uint32)
    int bytes = 0;

    bytes += sizeof(request->direccion_fisica);
    bytes += sizeof(request->cantidad_bytes);

    return bytes;
}

void *serializar_leerdato_response(t_memoria_leerdato_response *response, int *bytes)
{
    // TAM_BUFFER_RESPONSE (uint32), CANT_BYTES (uint32), DATOS (cant_bytes)
    uint32_t bytes_buffer_sin_tamanio = bytes_totales_leerdato_response_serializada(response);
    (*bytes) = sizeof(uint32_t) + bytes_buffer_sin_tamanio;
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    escribir_uint32(buffer, &desplazamiento, bytes_buffer_sin_tamanio);

    escribir_uint32(buffer, &desplazamiento, response->cantidad_bytes);
    escribir_buffer(buffer, &desplazamiento, response->dato, response->cantidad_bytes);

    return buffer;
}

t_memoria_leerdato_response *deserializar_leerdato_response(void *buffer)
{
    int desplazamiento = 0;

    uint32_t cantidad_bytes = leer_uint32(buffer, &desplazamiento);
    void *dato = leer_stream(buffer, cantidad_bytes, &desplazamiento);

    t_memoria_leerdato_response *response = leerdato_response_new(dato, cantidad_bytes);
    return response;
}

int bytes_totales_leerdato_response_serializada(t_memoria_leerdato_response *response)
{
    // CANT_BYTES (uint32), DATO (cant_bytes)
    int bytes = 0;

    bytes += sizeof(response->cantidad_bytes);
    bytes += response->cantidad_bytes;

    return bytes;
}
