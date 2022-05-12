#ifndef CPU_HANDSHAKEMEMORIA_H
#define CPU_HANDSHAKEMEMORIA_H

#include <serialization/serialization_utils.h>

#include <stdint.h>
#include <stdlib.h>

typedef struct t_cpu_handshakememoria_request
{
    uint32_t entradas_por_tabla;
    uint32_t tamanio_pagina;
} t_cpu_handshakememoria_request;

t_cpu_handshakememoria_request *handshakeconfiguraciones_request_new(uint32_t entradas_por_tabla, uint32_t tamanio_pagina);
void handshakeconfiguraciones_request_destroy(t_cpu_handshakememoria_request *request);

void *serializar_handshakeconfiguraciones_request(t_cpu_handshakememoria_request *request, int *bytes);
t_cpu_handshakememoria_request *deserializar_handshakeconfiguraciones_request(void *buffer);
int bytes_totales_handshakeconfiguraciones_request_serializada(t_cpu_handshakememoria_request *request);

#endif