#ifndef MEMORIA_MARCOPARAENTRADATABLA2_H
#define MEMORIA_MARCOPARAENTRADATABLA2_H

#include <serialization/serialization_utils.h>

#include <stdint.h>
#include <stdlib.h>

typedef struct t_memoria_marcoparaentradatabla2_request
{
    uint32_t numero_tablaprimernivel;
    uint32_t numero_tablasegundonivel;
    uint32_t entrada_tablasegundonivel;
} t_memoria_marcoparaentradatabla2_request;

typedef struct t_memoria_marcoparaentradatabla2_response
{
    uint32_t numero_marco;
    int32_t numero_pagina_reemplazada;
} t_memoria_marcoparaentradatabla2_response;

t_memoria_marcoparaentradatabla2_request *marcoparaentradatabla2_request_new(uint32_t numero_tablaprimernivel, uint32_t numero_tablasegundonivel, uint32_t entrada_tablasegundonivel);
void marcoparaentradatabla2_request_destroy(t_memoria_marcoparaentradatabla2_request *request);

t_memoria_marcoparaentradatabla2_response *marcoparaentradatabla2_response_new(uint32_t numero_marco, int32_t numero_pagina_reemplazada);
void marcoparaentradatabla2_response_destroy(t_memoria_marcoparaentradatabla2_response *response);

void *serializar_marcoparaentradatabla2_request(t_memoria_marcoparaentradatabla2_request *request, int *bytes);
t_memoria_marcoparaentradatabla2_request *deserializar_marcoparaentradatabla2_request(void *buffer);
int bytes_totales_marcoparaentradatabla2_request_serializada(t_memoria_marcoparaentradatabla2_request *request);

void *serializar_marcoparaentradatabla2_response(t_memoria_marcoparaentradatabla2_response *response, int *bytes);
t_memoria_marcoparaentradatabla2_response *deserializar_marcoparaentradatabla2_response(void *buffer);
int bytes_totales_marcoparaentradatabla2_response_serializada(t_memoria_marcoparaentradatabla2_response *response);

#endif
