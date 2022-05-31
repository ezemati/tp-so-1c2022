#ifndef CPU_CICLOINSTRUCCION_H
#define CPU_CICLOINSTRUCCION_H

#include <types/programa.h>

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

typedef enum ciclo_instruccion
{
   FETCH,
   DECODE,
   FETCH_OPERANDS,
   EXECUTE
} ciclo_instruccion;

typedef struct t_cpu_pcb
{
   uint32_t id;
   uint32_t tamanio;
   t_list *lista_instrucciones; 
   ciclo_instruccion estado;
   uint32_t program_counter;
   uint32_t tabla_paginas_primer_nivel;
   double estimacion_rafaga;
   uint32_t socket_consola;
   uint32_t bloqueo_pendiente; 
   time_t time_inicio_running;
   time_t time_fin_running;
   double milisegundos_en_running; 
} t_cpu_pcb;


void buscar_siguiente_instruccion(t_cpu_pcb *pcb);
void interpretar_instruccion(t_cpu_pcb *pcb);
void buscar_en_memoria_operandos(t_cpu_pcb *pcb);
void ejecutar_instruccion(t_cpu_pcb *pcb);
void check_interrupt(t_cpu_pcb *pcb); //VERIFICAR LO QUE TIENE QUE RECIBIR ES EL PCB

#endif
