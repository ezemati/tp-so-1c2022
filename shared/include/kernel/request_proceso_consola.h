#ifndef INCLUDE_KERNEL_REQUEST_PROCESO_CONSOLA_H_
#define INCLUDE_KERNEL_REQUEST_PROCESO_CONSOLA_H_

#include <serialization/serialization_utils.h>

#include <stdint.h>
#include <stdlib.h>

typedef struct t_kernel_pcb
{
    uint32_t id;
    uint32_t tamanio;
    char **lista_instrucciones;
    char *program_counter;
    uint32_t tabla_pagina;
    uint32_t estimacion_rafaga;
} t_kernel_pcb;

typedef struct t_kernel_request_proceso_consola
{
    uint32_t tamanio_proceso;
    char **lista_instrucciones;
} t_kernel_request_proceso_consola;

typedef struct t_kernel_response_proceso_consola
{
	t_kernel_pcb pcb;
} t_kernel_response_proceso_consola;

t_kernel_request_proceso_consola *request_proceso_consola(uint32_t tamanio_proceso, char **lista_instrucciones);
void request_proceso_consola_destroy(t_kernel_request_proceso_consola *request);

t_kernel_response_proceso_consola *response_proceso_consola(t_kernel_pcb pcb);
void response_proceso_consola_destroy(t_kernel_response_proceso_consola *response);

void *serializar_request_proceso_consola(t_kernel_request_proceso_consola *request, int *bytes);
t_kernel_request_proceso_consola *deserializar_request_proceso_consola(void *buffer);
int bytes_totales_request_proceso_consola_serializada(t_kernel_request_proceso_consola *request);

void *serializar_response_proceso_consola(t_kernel_response_proceso_consola *response, int *bytes);
t_kernel_response_proceso_consola *deserializar_response_proceso_consola(void *buffer);
int bytes_totales_response_proceso_consola_serializada(t_kernel_response_proceso_consola *response);


#endif
