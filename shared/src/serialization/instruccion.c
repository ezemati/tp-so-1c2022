#include <serialization/instruccion.h>

void *serializar_instruccion(t_instruccion *instruccion, int *bytes)
{
    // TAM_BUFFER_INSTRUCCION (uint32), TAM_CODIGO (uint32), CODIGO (TAM_CODIGO), CANT_PARAM (uint32)[, PARAM1 (uint32)[, PARAM2(uint32)]]
    uint32_t bytes_buffer_sin_tamanio = bytes_totales_instruccion_serializada(instruccion);
    (*bytes) = sizeof(uint32_t) + bytes_buffer_sin_tamanio;
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    escribir_uint32(buffer, &desplazamiento, bytes_buffer_sin_tamanio);

    escribir_string_con_longitud(buffer, &desplazamiento, instruccion->codigo_instruccion);

    escribir_uint32(buffer, &desplazamiento, instruccion->cantidad_parametros);
    for (int i = 0; i < instruccion->cantidad_parametros; i++)
    {
        escribir_uint32(buffer, &desplazamiento, instruccion->parametros[i]);
    }

    return buffer;
}

t_instruccion *deserializar_instruccion(void *buffer, int *desplazamiento, uint32_t *bytes)
{
    (*bytes) = leer_uint32(buffer, desplazamiento);

    t_instruccion *instruccion = malloc(*bytes);

    instruccion->codigo_instruccion = leer_string_con_longitud(buffer, desplazamiento);

    instruccion->cantidad_parametros = leer_uint32(buffer, desplazamiento);
    for (int i = 0; i < instruccion->cantidad_parametros; i++)
    {
        instruccion->parametros[i] = leer_uint32(buffer, desplazamiento);
    }

    return instruccion;
}

int bytes_totales_instruccion_serializada(t_instruccion *instruccion)
{
    // TAM_CODIGO (uint32), CODIGO (TAM_CODIGO), CANT_PARAM (uint32)[, PARAM1 (uint32)[, PARAM2(uint32)]]
    int bytes = 0;

    uint32_t tamanio_codigo = strlen(instruccion->codigo_instruccion) + 1;
    bytes += sizeof(tamanio_codigo);
    bytes += tamanio_codigo;

    uint32_t cant_parametros = instruccion->cantidad_parametros;
    bytes += sizeof(cant_parametros);                   // Cantidad de parametros
    bytes += sizeof(cant_parametros) * cant_parametros; // Cada parametro

    return bytes;
}
