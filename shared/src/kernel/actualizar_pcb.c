#include <kernel/actualizar_pcb.h>

t_kernel_actualizarpcb_request *actualizarpcb_request_new(uint32_t pid, uint32_t program_counter, uint32_t bloqueo_pendiente, time_miliseg time_inicio_running, time_miliseg time_fin_running)
{
    t_kernel_actualizarpcb_request *request = malloc(sizeof(t_kernel_actualizarpcb_request));
    request->pid = pid;
    request->program_counter = program_counter;
    request->bloqueo_pendiente = bloqueo_pendiente;
    request->time_inicio_running = time_inicio_running;
    request->time_fin_running = time_fin_running;
    return request;
}

void actualizarpcb_request_destroy(t_kernel_actualizarpcb_request *request)
{
    free(request);
}

t_kernel_actualizarpcb_response *actualizarpcb_response_new()
{
    t_kernel_actualizarpcb_response *response = malloc(sizeof(t_kernel_actualizarpcb_response));

    return response;
}

void actualizarpcb_response_destroy(t_kernel_actualizarpcb_response *response)
{
    free(response);
}

/***************************** SERIALIZACION *****************************/

void *serializar_actualizarpcb_request(t_kernel_actualizarpcb_request *request, int *bytes)
{
    // PID (uint32), PROGRAM_COUNTER (uint32), BLOQUEO_PENDIENTE (uint32), TIME_INICIO_RUNNING (time_miliseg), TIME_FIN_RUNNING (time_miliseg)
    (*bytes) = bytes_totales_actualizarpcb_request_serializada(request);
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    escribir_uint32_en_buffer(buffer, &desplazamiento, request->pid);
    escribir_uint32_en_buffer(buffer, &desplazamiento, request->program_counter);
    escribir_uint32_en_buffer(buffer, &desplazamiento, request->bloqueo_pendiente);
    escribir_en_buffer(buffer, &desplazamiento, &request->time_inicio_running, sizeof(request->time_inicio_running));
    escribir_en_buffer(buffer, &desplazamiento, &request->time_fin_running, sizeof(request->time_fin_running));

    return buffer;
}

t_kernel_actualizarpcb_request *deserializar_actualizarpcb_request(void *buffer)
{
    int desplazamiento = 0;

    uint32_t pid = leer_uint32_de_buffer(buffer, &desplazamiento);
    uint32_t program_counter = leer_uint32_de_buffer(buffer, &desplazamiento);
    uint32_t bloqueo_pendiente = leer_uint32_de_buffer(buffer, &desplazamiento);
    time_miliseg time_inicio_running, time_fin_running;
    leer_stream_de_buffer_noalloc(buffer, &time_inicio_running, sizeof(time_inicio_running), &desplazamiento);
    leer_stream_de_buffer_noalloc(buffer, &time_fin_running, sizeof(time_fin_running), &desplazamiento);

    t_kernel_actualizarpcb_request *request = actualizarpcb_request_new(pid, program_counter, bloqueo_pendiente, time_inicio_running, time_fin_running);

    return request;
}

int bytes_totales_actualizarpcb_request_serializada(t_kernel_actualizarpcb_request *request)
{
    // PID (uint32), PROGRAM_COUNTER (uint32), BLOQUEO_PENDIENTE (uint32), TIME_INICIO_RUNNING (time_miliseg), TIME_FIN_RUNNING (time_miliseg)
    int bytes = 0;

    bytes += sizeof(request->pid);
    bytes += sizeof(request->program_counter);
    bytes += sizeof(request->bloqueo_pendiente);
    bytes += sizeof(request->time_inicio_running);
    bytes += sizeof(request->time_fin_running);

    return bytes;
}

void *serializar_actualizarpcb_response(t_kernel_actualizarpcb_response *response, int *bytes)
{
    // ???
    (*bytes) = bytes_totales_actualizarpcb_response_serializada(response);
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    // ???

    return buffer;
}

t_kernel_actualizarpcb_response *deserializar_actualizarpcb_response(void *buffer)
{
    int desplazamiento = 0;

    // ???

    t_kernel_actualizarpcb_response *response = actualizarpcb_response_new();

    return response;
}

int bytes_totales_actualizarpcb_response_serializada(t_kernel_actualizarpcb_response *response)
{
    // CANT_BYTES (uint32)
    int bytes = 0;

    // ???

    return bytes;
}
