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

/***************************** SERIALIZACION *****************************/

void *serializar_inicializarproceso_request(t_memoria_inicializarproceso_request *request, int *bytes)
{
    // TAM_BUFFER_REQUEST (uint32), PID (uint32), TAM_PROCESO (uint32)
    uint32_t bytes_buffer_sin_tamanio = bytes_totales_inicializarproceso_request_serializada(request);
    (*bytes) = sizeof(uint32_t) + bytes_buffer_sin_tamanio;
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    escribir_uint32(buffer, &desplazamiento, bytes_buffer_sin_tamanio);

    escribir_uint32(buffer, &desplazamiento, request->pid);
    escribir_uint32(buffer, &desplazamiento, request->tamanio_proceso);

    return buffer;
}

t_memoria_inicializarproceso_request *deserializar_inicializarproceso_request(void *buffer)
{
    int desplazamiento = 0;

    uint32_t pid = leer_uint32(buffer, &desplazamiento);
    uint32_t tamanio_proceso = leer_uint32(buffer, &desplazamiento);

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
