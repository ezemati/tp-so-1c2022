#ifndef MEMORIA_ESCRIBIRDATO_H
#define MEMORIA_ESCRIBIRDATO_H

#include <serialization/serialization_utils.h>

#include <stdint.h>
#include <stdlib.h>

typedef struct t_memoria_escribirdato_request
{
    uint32_t pid;
    uint32_t direccion_fisica;
    uint32_t cantidad_bytes;
    void *dato;
} t_memoria_escribirdato_request;

typedef struct t_memoria_escribirdato_response
{
    uint32_t cantidad_bytes;
} t_memoria_escribirdato_response;

t_memoria_escribirdato_request *escribirdato_request_new(uint32_t pid, uint32_t direccion_fisica, uint32_t cantidad_bytes, void *dato);
void escribirdato_request_destroy(t_memoria_escribirdato_request *request);

t_memoria_escribirdato_response *escribirdato_response_new(uint32_t cantidad_bytes);
void escribirdato_response_destroy(t_memoria_escribirdato_response *response);

void *serializar_escribirdato_request(t_memoria_escribirdato_request *request, int *bytes);
t_memoria_escribirdato_request *deserializar_escribirdato_request(void *buffer);
int bytes_totales_escribirdato_request_serializada(t_memoria_escribirdato_request *request);

void *serializar_escribirdato_response(t_memoria_escribirdato_response *response, int *bytes);
t_memoria_escribirdato_response *deserializar_escribirdato_response(void *buffer);
int bytes_totales_escribirdato_response_serializada(t_memoria_escribirdato_response *response);

#endif
