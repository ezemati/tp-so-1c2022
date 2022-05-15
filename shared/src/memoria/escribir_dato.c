#include <memoria/escribir_dato.h>

t_memoria_escribirdato_request *escribirdato_request_new(uint32_t numero_tablasegundonivel, uint32_t entrada_tablasegundonivel, uint32_t direccion_fisica, uint32_t cantidad_bytes, void *dato)
{
    t_memoria_escribirdato_request *request = malloc(sizeof(t_memoria_escribirdato_request));
    request->numero_tablasegundonivel = numero_tablasegundonivel;
    request->entrada_tablasegundonivel = entrada_tablasegundonivel;
    request->direccion_fisica = direccion_fisica;
    request->cantidad_bytes = cantidad_bytes;

    request->dato = malloc(request->cantidad_bytes);
    memcpy(request->dato, dato, request->cantidad_bytes);

    return request;
}

void escribirdato_request_destroy(t_memoria_escribirdato_request *request)
{
    free(request->dato);
    free(request);
}

t_memoria_escribirdato_response *escribirdato_response_new(uint32_t cantidad_bytes)
{
    t_memoria_escribirdato_response *response = malloc(sizeof(t_memoria_escribirdato_response));
    response->cantidad_bytes = cantidad_bytes;
    return response;
}

void escribirdato_response_destroy(t_memoria_escribirdato_response *response)
{
    free(response);
}

/***************************** SERIALIZACION *****************************/

void *serializar_escribirdato_request(t_memoria_escribirdato_request *request, int *bytes)
{
    // TAM_BUFFER_REQUEST (uint32), NUMERO_SEGUNDONIVEL (uint32), ENTRADA_SEGUNDONIVEL (uint32), DIR_FISICA (uint32), CANT_BYTES (uint32), DATO (cant_bytes)
    uint32_t bytes_buffer_sin_tamanio = bytes_totales_escribirdato_request_serializada(request);
    (*bytes) = sizeof(uint32_t) + bytes_buffer_sin_tamanio;
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    escribir_uint32_en_buffer(buffer, &desplazamiento, bytes_buffer_sin_tamanio);

    escribir_uint32_en_buffer(buffer, &desplazamiento, request->numero_tablasegundonivel);
    escribir_uint32_en_buffer(buffer, &desplazamiento, request->entrada_tablasegundonivel);
    escribir_uint32_en_buffer(buffer, &desplazamiento, request->direccion_fisica);
    escribir_uint32_en_buffer(buffer, &desplazamiento, request->cantidad_bytes);
    escribir_en_buffer(buffer, &desplazamiento, request->dato, request->cantidad_bytes);

    return buffer;
}

t_memoria_escribirdato_request *deserializar_escribirdato_request(void *buffer)
{
    int desplazamiento = 0;

    uint32_t numero_tablasegundonivel = leer_uint32_de_buffer(buffer, &desplazamiento);
    uint32_t entrada_tablasegundonivel = leer_uint32_de_buffer(buffer, &desplazamiento);
    uint32_t direccion_fisica = leer_uint32_de_buffer(buffer, &desplazamiento);
    uint32_t cantidad_bytes = leer_uint32_de_buffer(buffer, &desplazamiento);
    void *dato = leer_stream_de_buffer(buffer, cantidad_bytes, &desplazamiento);

    t_memoria_escribirdato_request *request = escribirdato_request_new(numero_tablasegundonivel, entrada_tablasegundonivel, direccion_fisica, cantidad_bytes, dato);

    free(dato);

    return request;
}

int bytes_totales_escribirdato_request_serializada(t_memoria_escribirdato_request *request)
{
    // NUMERO_SEGUNDONIVEL (uint32), ENTRADA_SEGUNDONIVEL (uint32), DIR_FISICA (uint32), CANT_BYTES (uint32), DATO (cant_bytes)
    int bytes = 0;

    bytes += sizeof(request->numero_tablasegundonivel);
    bytes += sizeof(request->entrada_tablasegundonivel);
    bytes += sizeof(request->direccion_fisica);
    bytes += sizeof(request->cantidad_bytes);
    bytes += request->cantidad_bytes;

    return bytes;
}

void *serializar_escribirdato_response(t_memoria_escribirdato_response *response, int *bytes)
{
    // TAM_BUFFER_RESPONSE (uint32), CANT_BYTES (uint32)
    uint32_t bytes_buffer_sin_tamanio = bytes_totales_escribirdato_response_serializada(response);
    (*bytes) = sizeof(uint32_t) + bytes_buffer_sin_tamanio;
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    escribir_uint32_en_buffer(buffer, &desplazamiento, bytes_buffer_sin_tamanio);

    escribir_uint32_en_buffer(buffer, &desplazamiento, response->cantidad_bytes);

    return buffer;
}

t_memoria_escribirdato_response *deserializar_escribirdato_response(void *buffer)
{
    int desplazamiento = 0;

    uint32_t cantidad_bytes = leer_uint32_de_buffer(buffer, &desplazamiento);

    t_memoria_escribirdato_response *response = escribirdato_response_new(cantidad_bytes);

    return response;
}

int bytes_totales_escribirdato_response_serializada(t_memoria_escribirdato_response *response)
{
    // CANT_BYTES (uint32)
    int bytes = 0;

    bytes += sizeof(response->cantidad_bytes);

    return bytes;
}
