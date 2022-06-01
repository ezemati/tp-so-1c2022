#include <kernel_planifmedianoplazo.h>

static void thread_proceso_suspended_blocked(void *args);

void mediano_plazo_intentar_pasar_proceso_a_memoria()
{
    while (calcular_multiprogramacion() <= config->grado_multiprogramacion)
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

        log_info_if_logger_not_null(logger, "Pasando proceso %d desde SUSPENDED_READY a READY", pcb_suspended_ready->id);
        agregar_proceso_a_ready(pcb_suspended_ready);
    }
}

void suspender_proceso(t_kernel_pcb *pcb)
{
    pcb->estado = S_SUSPENDED_BLOCKED;

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

    if (!ok)
    {
        log_error_if_logger_not_null(logger, "Fallo memoria al suspender al proceso %d", pcb->id);
        return;
    }

    intentar_pasar_proceso_a_memoria(); // Este proceso paso a SUSPENDED_BLOCKED asi que dejo un espacio de multiprogramacion libre

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, (void *)thread_proceso_suspended_blocked, pcb);
    pthread_detach(thread_id);
}

static void thread_proceso_suspended_blocked(void *args)
{
    t_kernel_pcb *pcb = args;
    int tiempo_bloqueo = pcb->bloqueo_pendiente;
    int microsegundos = milisegundos_a_microsegundos(tiempo_bloqueo); // tiempo_bloqueo esta en milisegundos
    log_info_if_logger_not_null(logger, "Proceso %d entrando en suspension por %dms", pcb->id, tiempo_bloqueo);

    usleep(microsegundos);

    log_info_if_logger_not_null(logger, "Proceso %d saliendo de suspension", pcb->id);
    pcb->bloqueo_pendiente = 0;
    pcb->estado = S_SUSPENDED_READY;

    pthread_mutex_lock(&mutex_lista_suspended_ready);
    list_add(lista_suspended_ready, pcb);
    pthread_mutex_unlock(&mutex_lista_suspended_ready);

    intentar_pasar_proceso_a_memoria();
}
