#include <cpu_requests.h>

void atender_handshake_con_kernel(int socket_cliente)
{
    socket_conexion_kernel_dispatch = socket_cliente;

    uint32_t ok_recibido = 0, ok_enviado = 1;
    recibir_uint32_por_socket(socket_conexion_kernel_dispatch, &ok_recibido);
    if (ok_recibido != 1)
    {
        log_error_with_mutex(logger, &mutex_logger, "Fallo handshake con Kernel");
        exit(EXIT_FAILURE);
    }
    enviar_uint32_por_socket(socket_conexion_kernel_dispatch, ok_enviado);
}

void atender_ejecutar_proceso(int socket_cliente)
{
    pthread_mutex_lock(&mutex_hay_proceso_en_ejecucion);
    hay_proceso_en_ejecucion = true;
    pthread_mutex_unlock(&mutex_hay_proceso_en_ejecucion);

    // Reinicio el flag, por si quedo algun valor perdido
    pthread_mutex_lock(&mutex_hay_interrupcion);
    hay_interrupcion = false;
    pthread_mutex_unlock(&mutex_hay_interrupcion);

    void *buffer_request = NULL;
    recibir_buffer_con_bytes_por_socket(socket_cliente, &buffer_request);
    t_cpu_ejecutarproceso_request *request = deserializar_ejecutarproceso_request(buffer_request);

    tlb_clear(tlb);

    if (info_ejecucion_actual != NULL)
    {
        // Si no se liberaron los recursos del infoejecucionactual anterior, se liberan
        infoejecucionactual_destroy(info_ejecucion_actual);
    }
    info_ejecucion_actual = infoejecucionactual_new(request);

    ejecutarproceso_request_destroy(request);
    free(buffer_request);

    log_info_with_mutex(logger, &mutex_logger, "Empezando a ejecutar PID %d, con Program Counter %d", info_ejecucion_actual->pid, info_ejecucion_actual->program_counter);
    pthread_t thread_ejecucion_id;
    pthread_create(&thread_ejecucion_id, NULL, (void *)realizar_ejecucion, NULL);
    pthread_detach(thread_ejecucion_id);

    /*
    bool ok = true;
    t_cpu_ejecutarproceso_response *response = ejecutarproceso_response_new(ok);
    int bytes_response_serializada;
    void *response_serializada = serializar_ejecutarproceso_response(response, &bytes_response_serializada);
    enviar_buffer_serializado_con_bytes_por_socket(socket_cliente, response_serializada, bytes_response_serializada);
    free(response_serializada);
    ejecutarproceso_response_destroy(response);
    */
}
