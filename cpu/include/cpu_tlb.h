#ifndef CPU_TLB_H
#define CPU_TLB_H

#include <cpu_entradatlb.h>
#include <cpu_variables_globales.h>
#include <cpu_utils.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <commons/collections/list.h>

typedef struct t_cpu_tlb
{
    t_list *entradas_tlb; // De tipo t_cpu_entrada_tlb
    uint32_t cantidad_entradas_totales;
    uint32_t cantidad_entradas_ocupadas;
    uint32_t indice_reemplazo_fifo;
} t_cpu_tlb;

t_cpu_tlb *tlb_new(uint32_t cantidad_entradas_totales);
void tlb_destroy(t_cpu_tlb *tlb);
void tlb_clear(t_cpu_tlb *tlb);

/**
 * @brief Devuelve el numero de marco para el numero de pagina, o -1
 * si el numero de pagina no esta en la TLB
 */
int tlb_try_read_entry(t_cpu_tlb *tlb, uint32_t numero_pagina);

void tlb_add_entry(t_cpu_tlb *tlb, uint32_t numero_pagina, uint32_t numero_marco);

#endif
