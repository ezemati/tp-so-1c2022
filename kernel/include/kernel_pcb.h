#ifndef KERNEL_PCB_H
#define KERNEL_PCB_H

#include <kernel_config.h>
#include <kernel_variables_globales.h>
#include <types/programa.h>

#include <utils/converter.h>

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

typedef enum estado_proceso
{
    S_NEW = 1,
    S_READY,
    S_RUNNING,
    S_BLOCKED,
    S_SUSPENDED_READY,
    S_SUSPENDED_BLOCKED,
    S_EXIT
} estado_proceso;

typedef struct t_kernel_pcb
{
    uint32_t id;
    uint32_t tamanio;
    t_list *lista_instrucciones; // De tipo t_instruccion
    estado_proceso estado;
    uint32_t program_counter;
    uint32_t tabla_paginas_primer_nivel;
    double estimacion_rafaga;
    uint32_t socket_consola;
    uint32_t bloqueo_pendiente; // Indica la cantidad de milisegundos restantes que el proceso esta bloqueado
    time_t time_inicio_running;
    time_t time_fin_running;
    double milisegundos_en_running; // En caso de que el proceso sea desalojado y vuelva a ejecutar, esto guarda el acumulado
} t_kernel_pcb;

t_kernel_pcb *pcb_new(uint32_t pid, uint32_t socket_consola, t_programa *programa);
void pcb_destroy(t_kernel_pcb *pcb);
t_list *pcb_duplicar_instrucciones(t_programa *programa);
double tiempo_restante_segun_estimacion(t_kernel_pcb *self);
void cargar_tiempo_ejecucion_en_cpu(t_kernel_pcb *pcb, time_t time_inicio_running, time_t time_fin_running);

#endif