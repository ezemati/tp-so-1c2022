#ifndef CPU_TLB_H
#define CPU_TLB_H

// Esto es necesario por la referencia recursiva que hay entre t_cpu_tlb y t_cpu_entradatlb (CREO)
// (t_cpu_tlb referencia a t_cpu_entradatlb, t_cpu_entradatlb referencia a variables_globales,
// y variables_globales referencia a t_cpu_tlb)
struct t_cpu_entradatlb;

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

void tlb_replace_entry(t_cpu_tlb *tlb, struct t_cpu_entradatlb *entrada, uint32_t numero_pagina_nueva);

struct t_cpu_entradatlb *tlb_get_entry_con_numero_pagina(t_cpu_tlb *tlb, uint32_t numero_pagina);

void tlb_print_entradas(t_cpu_tlb *tlb);

#endif
