#include <cpu/ejecutar_proceso.h>

t_cpu_ejecutarproceso_request *ejecutarproceso_request_new(uint32_t pid, uint32_t program_counter, t_list *lista_instrucciones)
{
    t_cpu_ejecutarproceso_request *request = malloc(sizeof(t_cpu_ejecutarproceso_request));
    request->pid = pid;
    request->program_counter = program_counter;
    request->lista_instrucciones = instrucciones_duplicate(lista_instrucciones);
    return request;
}

void ejecutarproceso_request_destroy(t_cpu_ejecutarproceso_request *request)
{
    instrucciones_destroy(request->lista_instrucciones);
    free(request);
}

t_cpu_ejecutarproceso_response *ejecutarproceso_response_new(bool ok)
{
    t_cpu_ejecutarproceso_response *response = malloc(sizeof(t_cpu_ejecutarproceso_response));
    response->ok = ok;
    return response;
}

void ejecutarproceso_response_destroy(t_cpu_ejecutarproceso_response *response)
{
    free(response);
}

/***************************** SERIALIZACION *****************************/

void *serializar_ejecutarproceso_request(t_cpu_ejecutarproceso_request *request, int *bytes)
{
    // PID (uint32), PROGRAM_COUNTER (uint32), LISTA_INSTRUCCIONES
    (*bytes) = bytes_totales_ejecutarproceso_request_serializada(request);
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    escribir_uint32_en_buffer(buffer, &desplazamiento, request->pid);
    escribir_uint32_en_buffer(buffer, &desplazamiento, request->program_counter);

    serializar_lista_instrucciones_en_buffer(request->lista_instrucciones, buffer, &desplazamiento);

    return buffer;
}

t_cpu_ejecutarproceso_request *deserializar_ejecutarproceso_request(void *buffer)
{
    int desplazamiento = 0;

    uint32_t pid = leer_uint32_de_buffer(buffer, &desplazamiento);
    uint32_t program_counter = leer_uint32_de_buffer(buffer, &desplazamiento);
    t_list *lista_instrucciones = deserializar_lista_instrucciones(buffer, &desplazamiento);

    t_cpu_ejecutarproceso_request *request = ejecutarproceso_request_new(pid, program_counter, lista_instrucciones);

    instrucciones_destroy(lista_instrucciones); // En el new se duplica la lista, asi que hay que liberar la original

    return request;
}

int bytes_totales_ejecutarproceso_request_serializada(t_cpu_ejecutarproceso_request *request)
{
    // PID (uint32), PROGRAM_COUNTER (uint32), LISTA_INSTRUCCIONES
    int bytes = 0;

    bytes += sizeof(request->pid);
    bytes += sizeof(request->program_counter);
    bytes += bytes_totales_lista_instrucciones_serializada(request->lista_instrucciones);

    return bytes;
}

void *serializar_ejecutarproceso_response(t_cpu_ejecutarproceso_response *response, int *bytes)
{
    (*bytes) = bytes_totales_ejecutarproceso_response_serializada(response);
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    escribir_bool_en_buffer(buffer, &desplazamiento, response->ok);

    return buffer;
}

t_cpu_ejecutarproceso_response *deserializar_ejecutarproceso_response(void *buffer)
{
    int desplazamiento = 0;

    bool ok = leer_bool_de_buffer(buffer, &desplazamiento);

    t_cpu_ejecutarproceso_response *response = ejecutarproceso_response_new(ok);

    return response;
}

int bytes_totales_ejecutarproceso_response_serializada(t_cpu_ejecutarproceso_response *response)
{
    // OK (bool)
    int bytes = 0;

    bytes += sizeof(response->ok);

    return bytes;
}
