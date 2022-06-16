#ifndef CPU_ENTRADA_TLB_H
#define CPU_ENTRADA_TLB_H

#include <cpu_variables_globales.h>
#include <cpu_utils.h>

#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <commons/collections/list.h>

typedef struct t_cpu_entradatlb
{
    int32_t numero_pagina;
    int32_t numero_marco;
    time_miliseg last_used;
} t_cpu_entradatlb;

t_cpu_entradatlb *entradatlb_new();
void entradatlb_destroy(t_cpu_entradatlb *entrada);
void entradatlb_update(t_cpu_entradatlb *entrada, uint32_t numero_pagina, uint32_t numero_marco);
void entradatlb_clear(t_cpu_entradatlb *entrada);
void entradatlb_marcar_usada(t_cpu_entradatlb *entrada);

#endif
