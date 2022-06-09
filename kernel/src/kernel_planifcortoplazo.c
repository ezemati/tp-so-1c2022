#include <kernel_planifcortoplazo.h>

static t_kernel_pcb *obtener_proximo_para_ejecutar();
static t_kernel_pcb *obtener_proximo_para_ejecutar_fifo();
static t_kernel_pcb *obtener_proximo_para_ejecutar_srt();
static bool algoritmo_es_con_desalojo();

void agregar_proceso_a_ready(t_kernel_pcb *pcb)
{
    pthread_mutex_lock(&mutex_lista_ready);
    int index = list_size(lista_ready);
    pthread_mutex_unlock(&mutex_lista_ready);

    agregar_proceso_a_ready_en_index_sin_replanificar(pcb, index);

    if (algoritmo_es_con_desalojo() || !hay_proceso_en_ejecucion)
    {
        replanificar();
    }
}

void agregar_proceso_a_ready_en_index_sin_replanificar(t_kernel_pcb *pcb, int index)
{
    log_info_if_logger_not_null(logger, "Pasando proceso %d de %s a READY", pcb->id, estado_proceso_to_string(pcb->estado));

    pcb->estado = S_READY;

    pthread_mutex_lock(&mutex_lista_ready);
    list_add_in_index_custom(lista_ready, index, pcb);
    pthread_mutex_unlock(&mutex_lista_ready);
}

void replanificar()
{
    if (hay_proceso_en_ejecucion)
    {
        // Este if unicamente se ejecuta para planificacion SRT (porque en FIFO no hay
        // desalojo de CPU)
        t_kernel_pcb *pcb_desalojado = enviar_interrupcion_a_cpu();
        hay_proceso_en_ejecucion = false;

        // Para respetar el criterio CPU->IO->NEW, tengo que meter el proceso desalojado
        // en la actual ultima posicion de la lista_ready, asi el proceso que paso a READY
        // se desplaza en la lista y queda despues del proceso desalojado
        int ultimo_index = list_get_last_index(lista_ready);
        agregar_proceso_a_ready_en_index_sin_replanificar(pcb_desalojado, ultimo_index);
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
    void *pcb_con_tiempo_restante_minimo(void *element1, void *element2)
    {
        t_kernel_pcb *elementPcb1 = element1;
        t_kernel_pcb *elementPcb2 = element2;
        return tiempo_restante_segun_estimacion(elementPcb1) <= tiempo_restante_segun_estimacion(elementPcb2)
                   ? elementPcb1
                   : elementPcb2;
    }
    return list_get_minimum_or_null_if_empty(lista_ready, pcb_con_tiempo_restante_minimo);
}

static bool algoritmo_es_con_desalojo()
{
    return string_equals_ignore_case(config->algoritmo_planificacion, "SRT");
}
