#ifndef CPU_EJECUTARPROCESO_H
#define CPU_EJECUTARPROCESO_H

#include <serialization/serialization_utils.h>

#include <types/instruccion.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <commons/collections/list.h>

typedef struct t_cpu_ejecutarproceso_request
{
    uint32_t pid;
    uint32_t tamanio;
    uint32_t program_counter;
    t_list *lista_instrucciones; // De tipo t_instruccion
    uint32_t tabla_paginas_primer_nivel;
} t_cpu_ejecutarproceso_request;

typedef struct t_cpu_ejecutarproceso_response
{
    bool ok;
} t_cpu_ejecutarproceso_response;

t_cpu_ejecutarproceso_request *ejecutarproceso_request_new(uint32_t pid, uint32_t tamanio, uint32_t program_counter, uint32_t tabla_paginas_primer_nivel, t_list *lista_instrucciones);
void ejecutarproceso_request_destroy(t_cpu_ejecutarproceso_request *request);

t_cpu_ejecutarproceso_response *ejecutarproceso_response_new(bool ok);
void ejecutarproceso_response_destroy(t_cpu_ejecutarproceso_response *response);

void *serializar_ejecutarproceso_request(t_cpu_ejecutarproceso_request *request, int *bytes);
t_cpu_ejecutarproceso_request *deserializar_ejecutarproceso_request(void *buffer);
int bytes_totales_ejecutarproceso_request_serializada(t_cpu_ejecutarproceso_request *request);

void *serializar_ejecutarproceso_response(t_cpu_ejecutarproceso_response *response, int *bytes);
t_cpu_ejecutarproceso_response *deserializar_ejecutarproceso_response(void *buffer);
int bytes_totales_ejecutarproceso_response_serializada(t_cpu_ejecutarproceso_response *response);

#endif
