#ifndef KERNEL_PCB_H
#define KERNEL_PCB_H

#include <kernel_config.h>
#include <kernel_variables_globales.h>
#include <types/programa.h>

#include <stdint.h>
#include <stdlib.h>

typedef enum estado_proceso {
    NEW,
    READY,
    RUNNING,
    BLOCKED,
    SUSPENDED_READY,
    SUSPENDED_BLOCKED,
    EXIT
} estado_proceso;

typedef struct t_kernel_pcb
{
    uint32_t id;
    uint32_t tamanio;
    t_list *lista_instrucciones; // De tipo t_instruccion
    estado_proceso estado;
    uint32_t program_counter;
    uint32_t tabla_paginas_primer_nivel;
    uint32_t estimacion_rafaga;
} t_kernel_pcb;

t_kernel_pcb *pcb_new(uint32_t pid, t_programa *programa);
void pcb_destroy(t_kernel_pcb *pcb);
t_list *pcb_duplicar_instrucciones(t_programa *programa);

#endif