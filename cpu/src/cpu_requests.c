#include <cpu_requests.h>

void atender_handshake_con_kernel(int socket_cliente)
{
    socket_conexion_kernel = socket_cliente;

    uint32_t ok_recibido = 0, ok_enviado = 1;
    recibir_uint32_por_socket(socket_conexion_kernel, &ok_recibido);
    if (ok_recibido != 1)
    {
        log_error_if_logger_not_null(logger, "Fallo handshake con Kernel");
        exit(EXIT_FAILURE);
    }
    enviar_uint32_por_socket(socket_conexion_kernel, ok_enviado);
}

void atender_ejecutar_proceso(int socket_cliente)
{
    // Reinicio el flag, por si quedo algun valor perdido
    hay_interrupcion = false;

    void *buffer_request = NULL;
    recibir_buffer_con_bytes_por_socket(socket_cliente, &buffer_request);

    t_cpu_ejecutarproceso_request *request = deserializar_ejecutarproceso_request(buffer_request);

    info_ejecucion_actual = infoejecucionactual_new(request);

    log_debug(logger, "Empezando a ejecutar PID %d, con Program Counter %d", request->pid, request->program_counter);
    pthread_t thread_ejecucion_id;
    pthread_create(&thread_ejecucion_id, NULL, (void *)realizar_ejecucion, NULL);
    pthread_detach(thread_ejecucion_id);

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
