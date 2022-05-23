#ifndef CPU_RECIBIRINSTRUCCION_H
#define CPU_RECIBIRINSTRUCCION_H

#include <serialization/serialization_utils.h>

#include <stdint.h>
#include <stdlib.h>

typedef struct t_cpu_recibirinstruccion_response
{
    uint32_t id;
    uint32_t tamanio;
    void *instruccion;
    uint32_t program_counter;
    //uint32_t tabla_paginas; ?
    uint32_t estimacion_rafaga;
} t_cpu_recibirinstruccion_response;

t_cpu_recibirinstruccion_response *recibirinstruccion_response_new(uint32_t id, uint32_t tamanio, void *instruccion, uint32_t program_counter,uint32_t estimacion_rafaga);
void recibirinstruccion_response_destroy(t_cpu_recibirinstruccion_response *response);

void *serializar_recibirinstruccion_response(t_cpu_recibirinstruccion_response *response, int *bytes);
t_cpu_recibirinstruccion_response *deserializar_recibirinstruccion_response(void *buffer);
int bytes_totales_recibirinstruccion_response_serializada(t_cpu_recibirinstruccion_response *response);

#endif