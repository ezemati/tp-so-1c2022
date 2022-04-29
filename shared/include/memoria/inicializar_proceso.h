#ifndef MEMORIA_INICIALIZARPROCESO_H
#define MEMORIA_INICIALIZARPROCESO_H

#include <serialization/serialization_utils.h>

#include <stdint.h>
#include <stdlib.h>

typedef struct t_memoria_inicializarproceso_request
{
    uint32_t pid;
    uint32_t tamanio_proceso;
} t_memoria_inicializarproceso_request;

typedef struct t_memoria_inicializarproceso_response
{
    uint32_t numero_tablaprimernivel;
} t_memoria_inicializarproceso_response;

t_memoria_inicializarproceso_request *inicializarproceso_request_new(uint32_t pid, uint32_t tamanio_proceso);
void inicializarproceso_request_destroy(t_memoria_inicializarproceso_request *request);

t_memoria_inicializarproceso_response *inicializarproceso_response_new(uint32_t numero_tablaprimernivel);
void inicializarproceso_response_destroy(t_memoria_inicializarproceso_response *response);

void *serializar_inicializarproceso_request(t_memoria_inicializarproceso_request *request, int *bytes);
t_memoria_inicializarproceso_request *deserializar_inicializarproceso_request(void *buffer);
int bytes_totales_inicializarproceso_request_serializada(t_memoria_inicializarproceso_request *request);

void *serializar_inicializarproceso_response(t_memoria_inicializarproceso_response *response, int *bytes);
t_memoria_inicializarproceso_response *deserializar_inicializarproceso_response(void *buffer);
int bytes_totales_inicializarproceso_response_serializada(t_memoria_inicializarproceso_response *response);

#endif
