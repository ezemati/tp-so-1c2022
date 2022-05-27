#ifndef MEMORIA_SUSPENDERPROCESO_H
#define MEMORIA_SUSPENDERPROCESO_H

#include <serialization/serialization_utils.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct t_memoria_suspenderproceso_request
{
    uint32_t pid;
    uint32_t numero_tablaprimernivel;
} t_memoria_suspenderproceso_request;

typedef struct t_memoria_suspenderproceso_response
{
    bool ok;
} t_memoria_suspenderproceso_response;

t_memoria_suspenderproceso_request *suspenderproceso_request_new(uint32_t pid, uint32_t numero_tablaprimernivel);
void suspenderproceso_request_destroy(t_memoria_suspenderproceso_request *request);

t_memoria_suspenderproceso_response *suspenderproceso_response_new(bool ok);
void suspenderproceso_response_destroy(t_memoria_suspenderproceso_response *response);

void *serializar_suspenderproceso_request(t_memoria_suspenderproceso_request *request, int *bytes);
t_memoria_suspenderproceso_request *deserializar_suspenderproceso_request(void *buffer);
int bytes_totales_suspenderproceso_request_serializada(t_memoria_suspenderproceso_request *request);

void *serializar_suspenderproceso_response(t_memoria_suspenderproceso_response *response, int *bytes);
t_memoria_suspenderproceso_response *deserializar_suspenderproceso_response(void *buffer);
int bytes_totales_suspenderproceso_response_serializada(t_memoria_suspenderproceso_response *response);

#endif
