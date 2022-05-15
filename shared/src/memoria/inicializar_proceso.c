#include <memoria/inicializar_proceso.h>

t_memoria_inicializarproceso_request *inicializarproceso_request_new(uint32_t pid, uint32_t tamanio_proceso)
{
    t_memoria_inicializarproceso_request *request = malloc(sizeof(t_memoria_inicializarproceso_request));
    request->pid = pid;
    request->tamanio_proceso = tamanio_proceso;
    return request;
}

void inicializarproceso_request_destroy(t_memoria_inicializarproceso_request *request)
{
    free(request);
}

t_memoria_inicializarproceso_response *inicializarproceso_response_new(uint32_t numero_tablaprimernivel)
{
    t_memoria_inicializarproceso_response *response = malloc(sizeof(t_memoria_inicializarproceso_response));
    response->numero_tablaprimernivel = numero_tablaprimernivel;
    return response;
}

void inicializarproceso_response_destroy(t_memoria_inicializarproceso_response *response)
{
    free(response);
}

/***************************** SERIALIZACION *****************************/

void *serializar_inicializarproceso_request(t_memoria_inicializarproceso_request *request, int *bytes)
{
    // TAM_BUFFER_REQUEST (uint32), PID (uint32), TAM_PROCESO (uint32)
    uint32_t bytes_buffer_sin_tamanio = bytes_totales_inicializarproceso_request_serializada(request);
    (*bytes) = sizeof(uint32_t) + bytes_buffer_sin_tamanio;
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    escribir_uint32_en_buffer(buffer, &desplazamiento, bytes_buffer_sin_tamanio);

    escribir_uint32_en_buffer(buffer, &desplazamiento, request->pid);
    escribir_uint32_en_buffer(buffer, &desplazamiento, request->tamanio_proceso);

    return buffer;
}

t_memoria_inicializarproceso_request *deserializar_inicializarproceso_request(void *buffer)
{
    int desplazamiento = 0;

    uint32_t pid = leer_uint32_de_buffer(buffer, &desplazamiento);
    uint32_t tamanio_proceso = leer_uint32_de_buffer(buffer, &desplazamiento);

    t_memoria_inicializarproceso_request *request = inicializarproceso_request_new(pid, tamanio_proceso);
    return request;
}

int bytes_totales_inicializarproceso_request_serializada(t_memoria_inicializarproceso_request *request)
{
    // PID (uint32), TAM_PROCESO (uint32)
    int bytes = 0;

    bytes += sizeof(request->pid);
    bytes += sizeof(request->tamanio_proceso);

    return bytes;
}

void *serializar_inicializarproceso_response(t_memoria_inicializarproceso_response *response, int *bytes)
{
    // TAM_BUFFER_RESPONSE (uint32), NUMERO_PRIMER_NIVEL (uint32)
    uint32_t bytes_buffer_sin_tamanio = bytes_totales_inicializarproceso_response_serializada(response);
    (*bytes) = sizeof(uint32_t) + bytes_buffer_sin_tamanio;
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    escribir_uint32_en_buffer(buffer, &desplazamiento, bytes_buffer_sin_tamanio);

    escribir_uint32_en_buffer(buffer, &desplazamiento, response->numero_tablaprimernivel);

    return buffer;
}

t_memoria_inicializarproceso_response *deserializar_inicializarproceso_response(void *buffer)
{
    int desplazamiento = 0;

    uint32_t numero_tablaprimernivel = leer_uint32_de_buffer(buffer, &desplazamiento);

    t_memoria_inicializarproceso_response *response = inicializarproceso_response_new(numero_tablaprimernivel);
    return response;
}

int bytes_totales_inicializarproceso_response_serializada(t_memoria_inicializarproceso_response *response)
{
    // NUMERO_PRIMER_NIVEL (uint32)
    int bytes = 0;

    bytes += sizeof(response->numero_tablaprimernivel);

    return bytes;
}
