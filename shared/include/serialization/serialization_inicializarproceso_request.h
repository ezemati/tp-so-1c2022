#ifndef SERIALIZATION_INICIALIZARPROCESO_REQUEST_H
#define SERIALIZATION_INICIALIZARPROCESO_REQUEST_H

#include <types/inicializarproceso_request.h>
#include <serialization/serialization_utils.h>

#include <stdint.h>

void *serializar_inicializarproceso_request(t_inicializarproceso_request *request, int *bytes);
t_inicializarproceso_request *deserializar_inicializarproceso_request(void *buffer);
int bytes_totales_inicializarproceso_request_serializada(t_inicializarproceso_request *request);

#endif