#ifndef CPU_EJECUCION_INSTRUCCIONES_H
#define CPU_EJECUCION_INSTRUCCIONES_H

#include <types/instruccion.h>

#include <cpu_config.h>
#include <cpu_variables_globales.h>
#include <cpu_info_ejecucion_actual.h>

#include <utils/converter.h>

#include <stdint.h>
#include <unistd.h>

void ejecutar_instruccion(t_instruccion *instruccion, uint32_t valor_para_copy);
void ejecutar_noop(t_instruccion *instruccion);
void ejecutar_io(t_instruccion *instruccion);
void ejecutar_read(t_instruccion *instruccion);
void ejecutar_write(t_instruccion *instruccion);
void ejecutar_copy(t_instruccion *instruccion, uint32_t valor_a_copiar);
void ejecutar_exit(t_instruccion *instruccion);

#endif
