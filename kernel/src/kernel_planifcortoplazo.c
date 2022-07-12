#include <kernel_planifcortoplazo.h>

static t_kernel_pcb *obtener_proximo_para_ejecutar();
static t_kernel_pcb *obtener_proximo_para_ejecutar_fifo();
static t_kernel_pcb *obtener_proximo_para_ejecutar_srt();

void agregar_proceso_a_ready(t_kernel_pcb *pcb)
{
    estado_proceso estado_anterior = proceso_cambiar_estado(pcb, S_READY);

    log_info_with_mutex(logger, &mutex_logger, "Pasando proceso %d de %s a READY", pcb->id, estado_proceso_to_string(estado_anterior));

    list_add_with_mutex(lista_ready, pcb, &mutex_lista_ready);

    if (algoritmo_es_con_desalojo() || !sincro_test_bool(&hay_proceso_en_ejecucion, &mutex_hay_proceso_en_ejecucion))
    {
        replanificar();
    }
}

void agregar_proceso_a_ready_en_anteultima_posicion_sin_replanificar(t_kernel_pcb *pcb)
{
    estado_proceso estado_anterior = proceso_cambiar_estado(pcb, S_READY);

    log_info_with_mutex(logger, &mutex_logger, "Pasando proceso %d de %s a READY", pcb->id, estado_proceso_to_string(estado_anterior));

    void *pcb_que_paso_a_ready = list_remove_last_with_mutex(lista_ready, &mutex_lista_ready);
    list_add_with_mutex(lista_ready, pcb, &mutex_lista_ready);
    list_add_with_mutex(lista_ready, pcb_que_paso_a_ready, &mutex_lista_ready);
}

void replanificar()
{
    if (sincro_test_bool(&hay_proceso_en_ejecucion, &mutex_hay_proceso_en_ejecucion))
    {
        // Este if unicamente se ejecuta para planificacion SRT (porque en FIFO no hay
        // desalojo de CPU)
        t_kernel_pcb *pcb_desalojado = enviar_interrupcion_a_cpu();

        if (pcb_desalojado == NULL)
        {
            // Puede pasar que el proceso ya se haya desalojado a si mismo justo cuando se le manda
            // la interrupcion de desalojo. Si eso pasa, hay que cancelar la replanificacion en este hilo, porque
            // si el proceso se auto-desalojo (IO/EXIT) entonces ya se hace una replanificacion automatica
            log_error_with_mutex(logger, &mutex_logger, "Se mando una interrupcion de desalojo pero no habia ningun proceso ejecutandose en CPU");
            return;
        }

        sincro_set_bool(&hay_proceso_en_ejecucion, false, &mutex_hay_proceso_en_ejecucion);

        // Para respetar el criterio CPU->IO->NEW, tengo que sacar el PCB que esta en la ultima
        // posicion de la lista de READY (que seria el que paso de IO o NEW a READY y produjo esta
        // replanificacion), asi queda primero el proceso desalojado (que viene de CPU) y despues
        // queda el de IO/NEW que produjo la replanificacion
        agregar_proceso_a_ready_en_anteultima_posicion_sin_replanificar(pcb_desalojado);
    }

    t_kernel_pcb *pcb_a_ejecutar = obtener_proximo_para_ejecutar();
    if (pcb_a_ejecutar == NULL)
    {
        log_info_with_mutex(logger, &mutex_logger, "No hay ningun proceso para ejecutar...");
        return;
    }

    log_info_with_mutex(logger, &mutex_logger, "Proceso %d seleccionado para ejecutar", pcb_a_ejecutar->id);

    pthread_mutex_lock(&mutex_lista_ready);
    sacar_proceso_de_lista(lista_ready, pcb_a_ejecutar);
    pthread_mutex_unlock(&mutex_lista_ready);

    enviar_proceso_a_cpu_para_ejecucion(pcb_a_ejecutar);

    sincro_set_bool(&hay_proceso_en_ejecucion, true, &mutex_hay_proceso_en_ejecucion);
}

static t_kernel_pcb *obtener_proximo_para_ejecutar()
{
    print_procesos_listaready();

    t_kernel_pcb *pcb_a_ejecutar = config->algoritmo_planificacion == P_FIFO
                                       ? obtener_proximo_para_ejecutar_fifo()
                                       : obtener_proximo_para_ejecutar_srt();
    return pcb_a_ejecutar;
}

static t_kernel_pcb *obtener_proximo_para_ejecutar_fifo()
{
    t_kernel_pcb *proximo_fifo = list_get_first_element_with_mutex(lista_ready, &mutex_lista_ready);
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
    return list_get_minimum_or_null_if_empty_with_mutex(lista_ready, pcb_con_tiempo_restante_minimo, &mutex_lista_ready);
}
