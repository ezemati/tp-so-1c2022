#ifndef MEMORIA_LEERDATO_H
#define MEMORIA_LEERDATO_H

#include <serialization/serialization_utils.h>

#include <stdint.h>
#include <stdlib.h>

typedef struct t_memoria_leerdato_request
{
    uint32_t numero_tablasegundonivel;
    uint32_t entrada_tablasegundonivel;
    uint32_t direccion_fisica;
    uint32_t cantidad_bytes;
} t_memoria_leerdato_request;

typedef struct t_memoria_leerdato_response
{
    uint32_t cantidad_bytes;
    void *dato;
} t_memoria_leerdato_response;

t_memoria_leerdato_request *leerdato_request_new(uint32_t numero_tablasegundonivel, uint32_t entrada_tablasegundonivel, uint32_t direccion_fisica, uint32_t cantidad_bytes);
void leerdato_request_destroy(t_memoria_leerdato_request *request);

t_memoria_leerdato_response *leerdato_response_new(void *dato, uint32_t cantidad_bytes);
void leerdato_response_destroy(t_memoria_leerdato_response *response);

void *serializar_leerdato_request(t_memoria_leerdato_request *request, int *bytes);
t_memoria_leerdato_request *deserializar_leerdato_request(void *buffer);
int bytes_totales_leerdato_request_serializada(t_memoria_leerdato_request *request);

void *serializar_leerdato_response(t_memoria_leerdato_response *response, int *bytes);
t_memoria_leerdato_response *deserializar_leerdato_response(void *buffer);
int bytes_totales_leerdato_response_serializada(t_memoria_leerdato_response *response);

#endif
