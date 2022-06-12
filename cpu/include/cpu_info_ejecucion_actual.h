#ifndef CPU_INFO_EJECUCION_ACTUAL_H
#define CPU_INFO_EJECUCION_ACTUAL_H

#include <cpu/ejecutar_proceso.h>

#include <types/instruccion.h>

#include <serialization/serialization_utils.h>
#include <utils/time.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <commons/collections/list.h>

typedef struct t_cpu_info_ejecucion_actual
{
    uint32_t pid;
    uint32_t tamanio;
    uint32_t program_counter;
    uint32_t tabla_paginas_primer_nivel;
    uint32_t bloqueo_pendiente;
    t_list *lista_instrucciones; // De tipo t_instruccion
    double time_inicio_running;
    double time_fin_running;
    code_instruccion ultima_instruccion_ejecutada;
} t_cpu_info_ejecucion_actual;

t_cpu_info_ejecucion_actual *infoejecucionactual_new(t_cpu_ejecutarproceso_request *request);
void infoejecucionactual_destroy(t_cpu_info_ejecucion_actual *info_ejecucion_actual);

bool ejecucion_completada(t_cpu_info_ejecucion_actual *self);

#endif
