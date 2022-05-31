#ifndef CPU_EJECUCION_INSTRUCCIONES_H
#define CPU_EJECUCION_INSTRUCCIONES_H

#include <types/instruccion.h>

#include <stdint.h>

void ejecutar_instruccion(t_instruccion *instruccion, uint32_t valor_para_copy);
void ejecutar_noop();
void ejecutar_io(uint32_t duracion_io);
void ejecutar_read(uint32_t direccion_logica_lectura);
void ejecutar_write(uint32_t direccion_logica_escritura, uint32_t valor_a_escribir);
void ejecutar_copy(uint32_t direccion_logica_destino, uint32_t direccion_logica_origen, uint32_t valor_a_copiar);
void ejecutar_exit();

#endif
