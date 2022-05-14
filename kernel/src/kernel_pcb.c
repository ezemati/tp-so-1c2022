#include <kernel_pcb.h>

t_kernel_pcb *pcb_new(uint32_t pid, t_programa *programa)
{
    t_kernel_pcb *pcb = malloc(sizeof(t_kernel_pcb));
    pcb->id = pid;
    pcb->tamanio = programa->tamanio;
    pcb->lista_instrucciones = pcb_duplicar_instrucciones(programa);
    pcb->estado = NEW;
    pcb->estimacion_rafaga = config->estimacion_inicial; // Apenas se crea el PCB, la estimacion se inicializa con el valor inicial
    pcb->program_counter = 0;
    pcb->tabla_paginas_primer_nivel = 9999;
    return pcb;
}

void pcb_destroy(t_kernel_pcb *pcb)
{
    instrucciones_destroy(pcb->lista_instrucciones);
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
