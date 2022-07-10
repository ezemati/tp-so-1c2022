#include <kernel_pcb.h>

t_kernel_pcb *pcb_new(uint32_t pid, uint32_t socket_consola, t_programa *programa)
{
    t_kernel_pcb *pcb = malloc(sizeof(t_kernel_pcb));
    pcb->id = pid;
    pcb->tamanio = programa->tamanio;
    pcb->lista_instrucciones = pcb_duplicar_instrucciones(programa);
    pcb->estado = S_NEW;
    pthread_mutex_init(&(pcb->mutex_estado), NULL);
    pcb->program_counter = 0;
    pcb->tabla_paginas_primer_nivel = 9999;
    pcb->estimacion_rafaga = config->estimacion_inicial; // Apenas se crea el PCB, la estimacion se inicializa con el valor inicial
    pcb->socket_consola = socket_consola;
    pcb->bloqueo_pendiente = 0;
    pcb->milisegundos_en_running = 0;
    sem_init(&pcb->sem_suspended_blocked_memoria, 0, 0);
    return pcb;
}

void pcb_destroy(t_kernel_pcb *pcb)
{
    instrucciones_destroy(pcb->lista_instrucciones);
    sem_destroy(&pcb->sem_suspended_blocked_memoria);
    pthread_mutex_destroy(&(pcb->mutex_estado));
    free(pcb);
}

t_list *pcb_duplicar_instrucciones(t_programa *programa)
{
    t_list *instrucciones_pcb = list_create();

    t_list_iterator *iterator_instrucciones_programa = list_iterator_create(programa->instrucciones);
    while (list_iterator_has_next(iterator_instrucciones_programa))
    {
        t_instruccion *instruccion = (t_instruccion *)list_iterator_next(iterator_instrucciones_programa);
        t_instruccion *instruccion_duplicada = instruccion_duplicate(instruccion);
        list_add(instrucciones_pcb, instruccion_duplicada);
    }
    list_iterator_destroy(iterator_instrucciones_programa);

    return instrucciones_pcb;
}

void recalcular_estimacion(t_kernel_pcb *pcb)
{
    double alfa = config->alfa;
    time_miliseg estimacion_anterior = pcb->estimacion_rafaga;
    time_miliseg real_anterior = pcb->milisegundos_en_running;
    time_miliseg nueva_estimacion = alfa * real_anterior + (1 - alfa) * estimacion_anterior;
    pcb->estimacion_rafaga = nueva_estimacion;

    log_trace_with_mutex(logger, &mutex_logger, "Recalculando estimacion para proceso %d", pcb->id);
    log_trace_with_mutex(logger, &mutex_logger, "{ EstimacionAnterior=%lldms, RealAnterior=%lldms }", estimacion_anterior, real_anterior);
    log_info_with_mutex(logger, &mutex_logger, "Nueva estimacion para el proceso %d: %lldms", pcb->id, nueva_estimacion);
}

void actualizar_pcb_desalojado(t_kernel_pcb *pcb, uint32_t nuevo_program_counter, time_miliseg time_inicio_running, time_miliseg time_fin_running)
{
    pcb->program_counter = nuevo_program_counter;
    cargar_tiempo_ejecucion_en_cpu(pcb, time_inicio_running, time_fin_running);
}

void actualizar_pcb_bloqueado(t_kernel_pcb *pcb, uint32_t nuevo_program_counter, time_miliseg time_inicio_running, time_miliseg time_fin_running)
{
    pcb->program_counter = nuevo_program_counter;
    cargar_tiempo_ejecucion_en_cpu(pcb, time_inicio_running, time_fin_running);
    recalcular_estimacion(pcb);
}

bool proceso_tiene_estado(t_kernel_pcb *pcb, estado_proceso estado)
{
    pthread_mutex_lock(&(pcb->mutex_estado));
    bool ret = (pcb->estado == estado);
    pthread_mutex_unlock(&(pcb->mutex_estado));
    return ret;
}

estado_proceso proceso_cambiar_estado(t_kernel_pcb *pcb, estado_proceso nuevo_estado)
{
    pthread_mutex_lock(&(pcb->mutex_estado));
    estado_proceso estado_viejo = pcb->estado;
    pcb->estado = nuevo_estado;
    pthread_mutex_unlock(&(pcb->mutex_estado));
    return estado_viejo;
}

time_miliseg tiempo_restante_segun_estimacion(t_kernel_pcb *self)
{
    return self->estimacion_rafaga - self->milisegundos_en_running;
}

void cargar_tiempo_ejecucion_en_cpu(t_kernel_pcb *pcb, time_miliseg time_inicio_running, time_miliseg time_fin_running)
{
    time_miliseg milisegundos_running = milisegundos_entre_times(time_inicio_running, time_fin_running);
    pcb->milisegundos_en_running += milisegundos_running;
}

char *estado_proceso_del_proceso_to_string(t_kernel_pcb *pcb)
{
    pthread_mutex_lock(&(pcb->mutex_estado));
    char *estado = estado_proceso_to_string(pcb->estado);
    pthread_mutex_unlock(&(pcb->mutex_estado));
    return estado;
}

char *estado_proceso_to_string(estado_proceso estado)
{
    switch (estado)
    {
    case S_NEW:
        return "NEW";
    case S_READY:
        return "READY";
    case S_RUNNING:
        return "RUNNING";
    case S_BLOCKED:
        return "BLOCKED";
    case S_SUSPENDED_READY:
        return "SUSPENDED_READY";
    case S_SUSPENDED_BLOCKED:
        return "SUSPENDED_BLOCKED";
    case S_EXIT:
        return "EXIT";
    }

    return "ERROR";
}
