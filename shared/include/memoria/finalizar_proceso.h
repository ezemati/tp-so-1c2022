#ifndef MEMORIA_FINALIZARPROCESO_H
#define MEMORIA_FINALIZARPROCESO_H

#include <serialization/serialization_utils.h>

#include <stdint.h>
#include <stdlib.h>

typedef struct t_memoria_finalizarproceso_request
{
    uint32_t pid;
    uint32_t numero_tablaprimernivel;
} t_memoria_finalizarproceso_request;

typedef struct t_memoria_finalizarproceso_response
{

} t_memoria_finalizarproceso_response;

t_memoria_finalizarproceso_request *finalizarproceso_request_new(uint32_t pid, uint32_t numero_tablaprimernivel);
void finalizarproceso_request_destroy(t_memoria_finalizarproceso_request *request);

t_memoria_finalizarproceso_response *finalizarproceso_response_new();
void finalizarproceso_response_destroy(t_memoria_finalizarproceso_response *response);

void *serializar_finalizarproceso_request(t_memoria_finalizarproceso_request *request, int *bytes);
t_memoria_finalizarproceso_request *deserializar_finalizarproceso_request(void *buffer);
int bytes_totales_finalizarproceso_request_serializada(t_memoria_finalizarproceso_request *request);

void *serializar_finalizarproceso_response(t_memoria_finalizarproceso_response *response, int *bytes);
t_memoria_finalizarproceso_response *deserializar_finalizarproceso_response(void *buffer);
int bytes_totales_finalizarproceso_response_serializada(t_memoria_finalizarproceso_response *response);

#endif
