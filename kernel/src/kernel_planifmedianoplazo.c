#include <kernel_planifmedianoplazo.h>

bool mediano_plazo_intentar_pasar_proceso_a_memoria()
{
    bool se_agrego_proceso_a_memoria = false;

    while (puedo_pasar_proceso_a_memoria())
    {
        pthread_mutex_lock(&mutex_lista_suspended_ready);
        t_kernel_pcb *pcb_suspended_ready = list_get_first_element(lista_suspended_ready);
        pthread_mutex_unlock(&mutex_lista_suspended_ready);

        if (pcb_suspended_ready == NULL)
        {
            break;
        }

        pthread_mutex_lock(&mutex_lista_suspended_ready);
        sacar_proceso_de_lista(lista_suspended_ready, pcb_suspended_ready);
        pthread_mutex_unlock(&mutex_lista_suspended_ready);

        agregar_proceso_a_ready(pcb_suspended_ready);

        se_agrego_proceso_a_memoria = true;
    }

    return se_agrego_proceso_a_memoria;
}

void suspender_proceso(t_kernel_pcb *pcb, bool *se_paso_proceso_a_memoria)
{
    log_info_if_logger_not_null(logger, "Pasando proceso %d de %s a SUSPENDED_BLOCKED", pcb->id, estado_proceso_to_string(pcb->estado));

    pcb->estado = S_SUSPENDED_BLOCKED;

    int socket_memoria = crear_conexion(config->ip_memoria, config->puerto_memoria, NULL);

    t_memoria_suspenderproceso_request *request = suspenderproceso_request_new(pcb->id, pcb->tabla_paginas_primer_nivel);
    int bytes = 0;
    void *request_serializada = serializar_suspenderproceso_request(request, &bytes);
    enviar_buffer_serializado_con_instruccion_y_bytes_por_socket(socket_memoria, SUSPENDER_PROCESO, request_serializada, bytes);
    free(request_serializada);
    suspenderproceso_request_destroy(request);

    void *response_serializada = NULL;
    recibir_buffer_con_bytes_por_socket(socket_memoria, &response_serializada);
    t_memoria_suspenderproceso_response *response = deserializar_suspenderproceso_response(response_serializada);
    bool ok = response->ok;
    suspenderproceso_response_destroy(response);
    free(response_serializada);

    liberar_conexion(socket_memoria);

    if (!ok)
    {
        log_error_if_logger_not_null(logger, "Fallo memoria al suspender al proceso %d", pcb->id);
        return;
    }

    (*se_paso_proceso_a_memoria) = intentar_pasar_proceso_a_memoria(); // Este proceso paso a SUSPENDED_BLOCKED asi que dejo un espacio de multiprogramacion libre
}

void agregar_proceso_a_suspended_ready(t_kernel_pcb *pcb)
{
    log_info_if_logger_not_null(logger, "Pasando proceso %d de %s a SUSPENDED_READY", pcb->id, estado_proceso_to_string(pcb->estado));

    pcb->estado = S_SUSPENDED_READY;

    pthread_mutex_lock(&mutex_lista_suspended_ready);
    list_add(lista_suspended_ready, pcb);
    pthread_mutex_unlock(&mutex_lista_suspended_ready);

    intentar_pasar_proceso_a_memoria();
}
