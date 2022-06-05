#include <kernel_planifcortoplazo.h>

static t_kernel_pcb *obtener_proximo_para_ejecutar();
static t_kernel_pcb *obtener_proximo_para_ejecutar_fifo();
static t_kernel_pcb *obtener_proximo_para_ejecutar_srt();
static bool algoritmo_es_con_desalojo();

void agregar_proceso_a_ready(t_kernel_pcb *pcb)
{
    agregar_proceso_a_ready_sin_replanificar(pcb);

    if (algoritmo_es_con_desalojo() || !hay_proceso_en_ejecucion)
    {
        replanificar();
    }
}

void agregar_proceso_a_ready_sin_replanificar(t_kernel_pcb *pcb)
{
    log_info_if_logger_not_null(logger, "Pasando proceso %d de %s a READY", pcb->id, estado_proceso_to_string(pcb->estado));

    pcb->estado = S_READY;

    pthread_mutex_lock(&mutex_lista_ready);
    list_add(lista_ready, pcb);
    pthread_mutex_unlock(&mutex_lista_ready);
}

void replanificar()
{
    if (hay_proceso_en_ejecucion)
    {
        enviar_interrupcion_a_cpu();
        hay_proceso_en_ejecucion = false;
    }

    t_kernel_pcb *pcb_a_ejecutar = obtener_proximo_para_ejecutar();
    if (pcb_a_ejecutar == NULL)
    {
        log_info_if_logger_not_null(logger, "No hay ningun proceso para ejecutar...");
        return;
    }

    log_info_if_logger_not_null(logger, "Proceso %d seleccionado para ejecutar", pcb_a_ejecutar->id);

    pthread_mutex_lock(&mutex_lista_ready);
    sacar_proceso_de_lista(lista_ready, pcb_a_ejecutar);
    pthread_mutex_unlock(&mutex_lista_ready);

    enviar_proceso_a_cpu_para_ejecucion(pcb_a_ejecutar);
    hay_proceso_en_ejecucion = true;
}

static t_kernel_pcb *obtener_proximo_para_ejecutar()
{
    t_kernel_pcb *pcb_a_ejecutar = string_equals_ignore_case(config->algoritmo_planificacion, "FIFO")
                                       ? obtener_proximo_para_ejecutar_fifo()
                                       : obtener_proximo_para_ejecutar_srt();
    return pcb_a_ejecutar;
}

static t_kernel_pcb *obtener_proximo_para_ejecutar_fifo()
{
    pthread_mutex_lock(&mutex_lista_ready);
    t_kernel_pcb *proximo_fifo = list_get_first_element(lista_ready);
    pthread_mutex_unlock(&mutex_lista_ready);

    return proximo_fifo;
}

static t_kernel_pcb *obtener_proximo_para_ejecutar_srt()
{
    void *pcb_con_estimacion_minima(void *element1, void *element2)
    {
        t_kernel_pcb *elementPcb1 = element1;
        t_kernel_pcb *elementPcb2 = element2;
        return elementPcb1->estimacion_rafaga < elementPcb2->estimacion_rafaga
                   ? elementPcb1
                   : elementPcb2;
    }
    return list_get_minimum_or_null_if_empty(lista_ready, pcb_con_estimacion_minima);
}

static bool algoritmo_es_con_desalojo()
{
    return string_equals_ignore_case(config->algoritmo_planificacion, "SRT");
}
