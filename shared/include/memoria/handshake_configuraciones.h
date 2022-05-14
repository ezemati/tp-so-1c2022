#ifndef MEMORIA_HANDSHAKECONFIGURACIONES_H
#define MEMORIA_HANDSHAKECONFIGURACIONES_H

#include <serialization/serialization_utils.h>

#include <stdint.h>
#include <stdlib.h>

typedef struct t_memoria_handshakeconfiguraciones_response
{
    uint32_t entradas_por_tabla;
    uint32_t tamanio_pagina;
} t_memoria_handshakeconfiguraciones_response;

t_memoria_handshakeconfiguraciones_response *handshakeconfiguraciones_memoria_response_new(uint32_t entradas_por_tabla, uint32_t tamanio_pagina);
void handshakeconfiguraciones_memoria_response_destroy(t_memoria_handshakeconfiguraciones_response *response);

void *serializar_handshakeconfiguraciones_memoria_response(t_memoria_handshakeconfiguraciones_response *response, int *bytes);
t_memoria_handshakeconfiguraciones_response *deserializar_handshakeconfiguraciones_memoria_response(void *buffer);
int bytes_totales_handshakeconfiguraciones_memoria_response_serializada(t_memoria_handshakeconfiguraciones_response *response);

#endif
