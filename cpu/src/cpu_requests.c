#include <cpu_requests.h>

void atender_ejecutar_proceso(int socket_cliente)
{
    void *buffer_request = NULL;
    recibir_buffer_con_bytes_por_socket(socket_cliente, &buffer_request);

    t_cpu_ejecutarproceso_request *request = deserializar_ejecutarproceso_request(buffer_request);

    info_ejecucion_actual = infoejecucionactual_new(request);

    log_debug(logger, "Empezando a ejecutar PID %d, con Program Counter %d", request->pid, request->program_counter);

    bool ok = true;
    t_cpu_ejecutarproceso_response *response = ejecutarproceso_response_new(ok);
    int bytes_response_serializada;
    void *response_serializada = serializar_ejecutarproceso_response(response, &bytes_response_serializada);
    enviar_buffer_serializado_con_bytes_por_socket(socket_cliente, response_serializada, bytes_response_serializada);

    free(response_serializada);
    ejecutarproceso_response_destroy(response);
    ejecutarproceso_request_destroy(request);
    free(buffer_request);
}
