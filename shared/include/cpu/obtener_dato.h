#ifndef CPU_OBTENERDATO_H
#define CPU_OBTENERDATO_H

#include <serialization/serialization_utils.h>

#include <stdint.h>
#include <stdlib.h>

//Para mandar la direccion fisica a la memoria
typedef struct t_cpu_obtenerdato_request
{
    //uint32_t numero_tablasegundonivel;
    //uint32_t entrada_tablasegundonivel;
    uint32_t direccion_fisica;
    uint32_t cantidad_bytes;
} t_cpu_obtenerdato_request;

//Para recibir el valor de la dir fisica enviada
typedef struct t_cpu_obtenerdato_response
{
    uint32_t cantidad_bytes;
    void *dato;
} t_cpu_obtenerdato_response;


t_cpu_obtenerdato_request *obtenerdato_request_new(uint32_t direccion_fisica, uint32_t cantidad_bytes);
void obtenerdato_request_destroy(t_cpu_obtenerdato_request *request);


t_cpu_obtenerdato_response *obtenerdato_response_new(uint32_t cantidad_bytes, void *dato);
void obtenerdato_response_destroy(t_cpu_obtenerdato_response *response);

#endif