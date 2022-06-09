#ifndef CPU_MMU_H
#define CPU_MMU_H

#include <cpu_utils.h>

bool direccion_logica_valida(uint32_t direccion_logica);
uint32_t traducir_direccion_logica_a_fisica(uint32_t direccion_logica);
uint32_t fetch_valor_para_copy(uint32_t direccion_logica_origen);

#endif
