#include <memoria/finalizar_proceso.h>

t_memoria_finalizarproceso_request *finalizarproceso_request_new(uint32_t pid, uint32_t numero_tablaprimernivel)
{
    t_memoria_finalizarproceso_request *request = malloc(sizeof(t_memoria_finalizarproceso_request));
    request->pid = pid;
    request->numero_tablaprimernivel = numero_tablaprimernivel;
    return request;
}

void finalizarproceso_request_destroy(t_memoria_finalizarproceso_request *request)
{
    free(request);
}

t_memoria_finalizarproceso_response *finalizarproceso_response_new()
{
    t_memoria_finalizarproceso_response *response = malloc(sizeof(t_memoria_finalizarproceso_response));

    return response;
}

void finalizarproceso_response_destroy(t_memoria_finalizarproceso_response *response)
{
    free(response);
}

/***************************** SERIALIZACION *****************************/

void *serializar_finalizarproceso_request(t_memoria_finalizarproceso_request *request, int *bytes)
{
    // TAM_BUFFER_REQUEST (uint32), PID (uint32), NUMERO_PRIMER_NIVEL (uint32)
    uint32_t bytes_buffer_sin_tamanio = bytes_totales_finalizarproceso_request_serializada(request);
    (*bytes) = sizeof(uint32_t) + bytes_buffer_sin_tamanio;
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    escribir_uint32(buffer, &desplazamiento, bytes_buffer_sin_tamanio);

    escribir_uint32(buffer, &desplazamiento, request->pid);
    escribir_uint32(buffer, &desplazamiento, request->numero_tablaprimernivel);

    return buffer;
}

t_memoria_finalizarproceso_request *deserializar_finalizarproceso_request(void *buffer)
{
    int desplazamiento = 0;

    uint32_t pid = leer_uint32(buffer, &desplazamiento);
    uint32_t numero_tablaprimernivel = leer_uint32(buffer, &desplazamiento);

    t_memoria_finalizarproceso_request *request = finalizarproceso_request_new(pid, numero_tablaprimernivel);
    return request;
}

int bytes_totales_finalizarproceso_request_serializada(t_memoria_finalizarproceso_request *request)
{
    // PID (uint32), NUMERO_PRIMER_NIVEL (uint32)
    int bytes = 0;

    bytes += sizeof(request->pid);
    bytes += sizeof(request->numero_tablaprimernivel);

    return bytes;
}

void *serializar_finalizarproceso_response(t_memoria_finalizarproceso_response *response, int *bytes)
{
    // TAM_BUFFER_RESPONSE (uint32), ___
    uint32_t bytes_buffer_sin_tamanio = bytes_totales_finalizarproceso_response_serializada(response);
    (*bytes) = sizeof(uint32_t) + bytes_buffer_sin_tamanio;
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    escribir_uint32(buffer, &desplazamiento, bytes_buffer_sin_tamanio);

    // ???

    return buffer;
}

t_memoria_finalizarproceso_response *deserializar_finalizarproceso_response(void *buffer)
{
    int desplazamiento = 0;

    // ???

    t_memoria_finalizarproceso_response *response = finalizarproceso_response_new();
    return response;
}

int bytes_totales_finalizarproceso_response_serializada(t_memoria_finalizarproceso_response *response)
{
    // ___
    int bytes = 0;

    // ???

    return bytes;
}
