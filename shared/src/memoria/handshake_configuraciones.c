#include <memoria/handshake_configuraciones.h>

t_memoria_handshakeconfiguraciones_response *handshakeconfiguraciones_memoria_response_new(uint32_t entradas_por_tabla, uint32_t tamanio_pagina)
{
    t_memoria_handshakeconfiguraciones_response *response = malloc(sizeof(t_memoria_handshakeconfiguraciones_response));
    response->entradas_por_tabla = entradas_por_tabla;
    response->tamanio_pagina = tamanio_pagina;
    return response;
}

void handshakeconfiguraciones_memoria_response_destroy(t_memoria_handshakeconfiguraciones_response *response)
{
    free(response);
}

/***************************** SERIALIZACION *****************************/

void *serializar_handshakeconfiguraciones_memoria_response(t_memoria_handshakeconfiguraciones_response *response, int *bytes)
{
    // TAM_BUFFER_RESPONSE (uint32), ENTRADAS_POR_TABLA (uint32), TAMANIO_PAGINA (uint32)
    uint32_t bytes_buffer_sin_tamanio = bytes_totales_handshakeconfiguraciones_memoria_response_serializada(response);
    (*bytes) = sizeof(uint32_t) + bytes_buffer_sin_tamanio;
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    escribir_uint32_en_buffer(buffer, &desplazamiento, bytes_buffer_sin_tamanio);

    escribir_uint32_en_buffer(buffer, &desplazamiento, response->entradas_por_tabla);
    escribir_uint32_en_buffer(buffer, &desplazamiento, response->tamanio_pagina);

    return buffer;
}

t_memoria_handshakeconfiguraciones_response *deserializar_handshakeconfiguraciones_memoria_response(void *buffer)
{
    int desplazamiento = 0;

    uint32_t entradas_por_tabla = leer_uint32_de_buffer(buffer, &desplazamiento);
    uint32_t tamanio_pagina = leer_uint32_de_buffer(buffer, &desplazamiento);

    t_memoria_handshakeconfiguraciones_response *response = handshakeconfiguraciones_memoria_response_new(entradas_por_tabla, tamanio_pagina);

    return response;
}

int bytes_totales_handshakeconfiguraciones_memoria_response_serializada(t_memoria_handshakeconfiguraciones_response *response)
{
    // ENTRADAS_POR_TABLA (uint32), TAMANIO_PAGINA (uint32)
    int bytes = 0;

    bytes += sizeof(response->entradas_por_tabla);
    bytes += sizeof(response->tamanio_pagina);

    return bytes;
}
