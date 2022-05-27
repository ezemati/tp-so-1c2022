#include <kernel_planifcortoplazo.h>

static void replanificar();
static t_kernel_pcb *obtener_proximo_para_ejecutar();
static t_kernel_pcb *obtener_proximo_para_ejecutar_fifo();
static t_kernel_pcb *obtener_proximo_para_ejecutar_srt();
static bool algoritmo_es_con_desalojo();
static void thread_proceso_blocked(void *args);

void agregar_proceso_a_ready(t_kernel_pcb *pcb)
{
    pcb->estado = S_READY;

    pthread_mutex_lock(&mutex_lista_ready);
    list_add(lista_ready, pcb);
    pthread_mutex_unlock(&mutex_lista_ready);

    if (algoritmo_es_con_desalojo())
    {
        replanificar();
    }
}

void bloquear_proceso(t_kernel_pcb *pcb)
{
    pcb->estado = S_BLOCKED;
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, (void *)thread_proceso_blocked, pcb);
    pthread_detach(thread_id);
}

static void replanificar()
{
    enviar_interrupcion_a_cpu();

    t_kernel_pcb *pcb_a_ejecutar = obtener_proximo_para_ejecutar();

    pthread_mutex_lock(&mutex_lista_ready);
    sacar_proceso_de_lista(lista_ready, pcb_a_ejecutar);
    pthread_mutex_unlock(&mutex_lista_ready);

    // pcb_a_ejecutar->estado = S_RUNNING;

    enviar_nuevo_proceso_a_cpu(pcb_a_ejecutar);
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
        return elementPcb1->estimacion_rafaga <= elementPcb2->estimacion_rafaga
                   ? elementPcb1
                   : elementPcb2;
    }
    return list_get_minimum(lista_ready, pcb_con_estimacion_minima);
}

static bool algoritmo_es_con_desalojo()
{
    return string_equals_ignore_case(config->algoritmo_planificacion, "SRT");
}

static void thread_proceso_blocked(void *args)
{
    t_kernel_pcb *pcb = args;
    int tiempo_bloqueo = pcb->bloqueo_pendiente;
    int microsegundos = tiempo_bloqueo * 1000; // tiempo_bloqueo esta en milisegundos
    log_info_if_logger_not_null(logger, "Proceso %d entrando en bloqueo por %dms", pcb->id, tiempo_bloqueo);

    usleep(microsegundos);

    log_info_if_logger_not_null(logger, "Proceso %d saliendo de bloqueo y pasando a READY", pcb->id);
    pcb->bloqueo_pendiente = 0;
    agregar_proceso_a_ready(pcb);
}
