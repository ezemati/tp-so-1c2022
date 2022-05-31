#include <cpu_requests.h>

void atender_ejecutar_proceso(int socket_cliente)
{
    void *buffer_request = NULL;
    recibir_buffer_con_bytes_por_socket(socket_cliente, &buffer_request);

    t_cpu_ejecutarproceso_request *request = deserializar_ejecutarproceso_request(buffer_request);

    log_debug(logger, "Empezando a ejecutar PID %d, con Program Counter %d", request->pid, request->program_counter);

    free(buffer_request);
}
