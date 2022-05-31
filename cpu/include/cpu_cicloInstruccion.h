#ifndef CPU_CICLOINSTRUCCION_H
#define CPU_CICLOINSTRUCCION_H

#include <types/programa.h>

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

typedef enum ciclo_proceso
{
   FETCH,
   DECODE,
   FETCHOPERANDS,
   EXECUTE
} ciclo_proceso;

typedef struct t_cpu_pcb
{
   uint32_t id;
   uint32_t tamanio;
   t_list *lista_instrucciones; // De tipo t_instruccion
   ciclo_proceso estado;
   uint32_t program_counter;
   uint32_t tabla_paginas_primer_nivel;
   double estimacion_rafaga;
   uint32_t socket_consola;
   uint32_t bloqueo_pendiente; // Indica la cantidad de milisegundos restantes que el proceso esta bloqueado
   time_t time_inicio_running;
   time_t time_fin_running;
   double milisegundos_en_running; // En caso de que el proceso sea desalojado y vuelva a ejecutar, esto guarda el acumulado
} t_cpu_pcb;

bool proxima_instruccion(t_list lista_instrucciones);

#endif
