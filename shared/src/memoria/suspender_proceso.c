#include <memoria/suspender_proceso.h>

t_memoria_suspenderproceso_request *suspenderproceso_request_new(uint32_t pid, uint32_t numero_tablaprimernivel)
{
    t_memoria_suspenderproceso_request *request = malloc(sizeof(t_memoria_suspenderproceso_request));
    request->pid = pid;
    request->numero_tablaprimernivel = numero_tablaprimernivel;
    return request;
}

void suspenderproceso_request_destroy(t_memoria_suspenderproceso_request *request)
{
    free(request);
}

t_memoria_suspenderproceso_response *suspenderproceso_response_new(bool ok)
{
    t_memoria_suspenderproceso_response *response = malloc(sizeof(t_memoria_suspenderproceso_response));
    response->ok = ok;
    return response;
}

void suspenderproceso_response_destroy(t_memoria_suspenderproceso_response *response)
{
    free(response);
}

/***************************** SERIALIZACION *****************************/

void *serializar_suspenderproceso_request(t_memoria_suspenderproceso_request *request, int *bytes)
{
    // PID (uint32), NUMERO_PRIMER_NIVEL (uint32)
    (*bytes) = bytes_totales_suspenderproceso_request_serializada(request);
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    escribir_uint32_en_buffer(buffer, &desplazamiento, request->pid);
    escribir_uint32_en_buffer(buffer, &desplazamiento, request->numero_tablaprimernivel);

    return buffer;
}

t_memoria_suspenderproceso_request *deserializar_suspenderproceso_request(void *buffer)
{
    int desplazamiento = 0;

    uint32_t pid = leer_uint32_de_buffer(buffer, &desplazamiento);
    uint32_t numero_tablaprimernivel = leer_uint32_de_buffer(buffer, &desplazamiento);

    t_memoria_suspenderproceso_request *request = suspenderproceso_request_new(pid, numero_tablaprimernivel);
    return request;
}

int bytes_totales_suspenderproceso_request_serializada(t_memoria_suspenderproceso_request *request)
{
    // PID (uint32), NUMERO_PRIMER_NIVEL (uint32)
    int bytes = 0;

    bytes += sizeof(request->pid);
    bytes += sizeof(request->numero_tablaprimernivel);

    return bytes;
}

void *serializar_suspenderproceso_response(t_memoria_suspenderproceso_response *response, int *bytes)
{
    // OK (bool)
    (*bytes) = bytes_totales_suspenderproceso_response_serializada(response);
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    escribir_bool_en_buffer(buffer, &desplazamiento, response->ok);

    return buffer;
}

t_memoria_suspenderproceso_response *deserializar_suspenderproceso_response(void *buffer)
{
    int desplazamiento = 0;

    bool ok = leer_bool_de_buffer(buffer, &desplazamiento);

    t_memoria_suspenderproceso_response *response = suspenderproceso_response_new(ok);
    return response;
}

int bytes_totales_suspenderproceso_response_serializada(t_memoria_suspenderproceso_response *response)
{
    // OK (bool)
    int bytes = 0;

    bytes += sizeof(response->ok);

    return bytes;
}
