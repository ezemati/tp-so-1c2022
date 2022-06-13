#ifndef KERNEL_ACTUALIZARPCB_H
#define KERNEL_ACTUALIZARPCB_H

#include <serialization/serialization_utils.h>
#include <types/time_types.h>

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

typedef struct t_kernel_actualizarpcb_request
{
    uint32_t pid;
    uint32_t program_counter;
    uint32_t bloqueo_pendiente;
    time_miliseg time_inicio_running;
    time_miliseg time_fin_running;
} t_kernel_actualizarpcb_request;

typedef struct t_kernel_actualizarpcb_response
{

} t_kernel_actualizarpcb_response;

t_kernel_actualizarpcb_request *actualizarpcb_request_new(uint32_t pid, uint32_t program_counter, uint32_t bloqueo_pendiente, time_miliseg time_inicio_running, time_miliseg time_fin_running);
void actualizarpcb_request_destroy(t_kernel_actualizarpcb_request *request);

t_kernel_actualizarpcb_response *actualizarpcb_response_new();
void actualizarpcb_response_destroy(t_kernel_actualizarpcb_response *response);

void *serializar_actualizarpcb_request(t_kernel_actualizarpcb_request *request, int *bytes);
t_kernel_actualizarpcb_request *deserializar_actualizarpcb_request(void *buffer);
int bytes_totales_actualizarpcb_request_serializada(t_kernel_actualizarpcb_request *request);

void *serializar_actualizarpcb_response(t_kernel_actualizarpcb_response *response, int *bytes);
t_kernel_actualizarpcb_response *deserializar_actualizarpcb_response(void *buffer);
int bytes_totales_actualizarpcb_response_serializada(t_kernel_actualizarpcb_response *response);

#endif
