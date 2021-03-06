#ifndef CPU_CONFIG_H
#define CPU_CONFIG_H

#include <types/time_types.h>
#include <utils/logs.h>

#include <stdint.h>
#include <stdlib.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/string.h>

typedef enum algoritmo_reemplazo_tlb
{
    T_FIFO = 1,
    T_LRU
} algoritmo_reemplazo_tlb;

typedef struct t_cpu_config
{
    uint32_t entradas_tlb;
    algoritmo_reemplazo_tlb reemplazo_tlb;
    time_miliseg retardo_noop;
    char *ip_memoria;
    uint32_t puerto_memoria;
    uint32_t puerto_escucha_dispatch;
    uint32_t puerto_escucha_interrupt;

    uint32_t memoria_entradas_por_tabla;
    uint32_t memoria_tamanio_pagina;
} t_cpu_config;

t_cpu_config *cpu_config_new(char *config_path);
void cpu_config_destroy(t_cpu_config *config);

#endif
