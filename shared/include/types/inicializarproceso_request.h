#ifndef TYPES_INICIALIZARPROCESO_REQUEST_H
#define TYPES_INICIALIZARPROCESO_REQUEST_H

#include <stdint.h>
#include <stdlib.h>

typedef struct t_inicializarproceso_request
{
    uint32_t pid;
    uint32_t tamanio_proceso;
} t_inicializarproceso_request;

t_inicializarproceso_request *inicializarproceso_request_new(uint32_t pid, uint32_t tamanio_proceso);
void inicializarproceso_request_destroy(t_inicializarproceso_request *request);

#endif
