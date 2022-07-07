#include <kernel_planiflargoplazo.h>

static void pasar_proceso_new_a_ready(t_kernel_pcb *pcb);
static void finalizar_proceso_en_memoria(t_kernel_pcb *pcb);
static void finalizar_proceso_en_consola(t_kernel_pcb *pcb);

bool largo_plazo_intentar_pasar_proceso_a_memoria()
{
    bool se_agrego_proceso_a_memoria = false;

    while (puedo_pasar_proceso_a_memoria())
    {
        t_kernel_pcb *pcb_new = list_get_first_element_with_mutex(lista_new, &mutex_lista_new);

        if (pcb_new == NULL)
            break;

        pthread_mutex_lock(&mutex_lista_new);
        sacar_proceso_de_lista(lista_new, pcb_new);
        pthread_mutex_unlock(&mutex_lista_new);

        pasar_proceso_new_a_ready(pcb_new);

        se_agrego_proceso_a_memoria = true;
    }

    return se_agrego_proceso_a_memoria;
}

void finalizar_proceso(t_kernel_pcb *pcb)
{
    log_info_if_logger_not_null(logger, "Pasando proceso %d de %s a EXIT", pcb->id, estado_proceso_to_string(pcb->estado));

    pcb->estado = S_EXIT;
    finalizar_proceso_en_memoria(pcb);
    finalizar_proceso_en_consola(pcb);

    pthread_mutex_lock(&mutex_lista_procesos);
    sacar_proceso_de_lista(lista_procesos, pcb);
    pthread_mutex_unlock(&mutex_lista_procesos);

    pcb_destroy(pcb);

    bool se_paso_proceso_a_memoria = intentar_pasar_proceso_a_memoria();
    if (!se_paso_proceso_a_memoria)
    {
        // Si se pudo pasar un proceso a memoria, entonces se replanifica automaticamente,
        // asi que hay que replanificar aca a mano unicamente si no se pudo pasar ningun proceso
        replanificar();
    }
}

static void pasar_proceso_new_a_ready(t_kernel_pcb *pcb)
{
    log_info_if_logger_not_null(logger, "Inicializando proceso %d en Memoria", pcb->id);

    int socket_memoria = crear_conexion(config->ip_memoria, config->puerto_memoria, NULL);

    t_memoria_inicializarproceso_request *request = inicializarproceso_request_new(pcb->id, pcb->tamanio);
    int bytes_request_serializada = 0;
    void *request_serializada = serializar_inicializarproceso_request(request, &bytes_request_serializada);
    enviar_buffer_serializado_con_instruccion_y_bytes_por_socket(socket_memoria, INICIALIZAR_PROCESO, request_serializada, bytes_request_serializada);
    free(request_serializada);
    inicializarproceso_request_destroy(request);

    void *response_serializada = NULL;
    recibir_buffer_con_bytes_por_socket(socket_memoria, &response_serializada);
    t_memoria_inicializarproceso_response *response = deserializar_inicializarproceso_response(response_serializada);
    pcb->tabla_paginas_primer_nivel = response->numero_tablaprimernivel;
    inicializarproceso_response_destroy(response);
    free(response_serializada);

    liberar_conexion(socket_memoria);

    agregar_proceso_a_ready(pcb);
}

static void finalizar_proceso_en_memoria(t_kernel_pcb *pcb)
{
    int socket_memoria = crear_conexion(config->ip_memoria, config->puerto_memoria, NULL);

    t_memoria_finalizarproceso_request *request = finalizarproceso_request_new(pcb->id, pcb->tabla_paginas_primer_nivel);
    int bytes_request_serializada = 0;
    void *request_serializada = serializar_finalizarproceso_request(request, &bytes_request_serializada);
    enviar_buffer_serializado_con_instruccion_y_bytes_por_socket(socket_memoria, FINALIZAR_PROCESO, request_serializada, bytes_request_serializada);
    free(request_serializada);
    finalizarproceso_request_destroy(request);
}

static void finalizar_proceso_en_consola(t_kernel_pcb *pcb)
{
    uint32_t proceso_finalizado_ok = 1;
    enviar_uint32_por_socket(pcb->socket_consola, proceso_finalizado_ok);
    liberar_conexion(pcb->socket_consola);
}
