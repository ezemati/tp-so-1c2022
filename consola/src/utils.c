#include "utils.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <commons/collections/list.h>

void *serializar_instrucciones(t_list *instrucciones, uint32_t tamanio_programa)
{
    int bytes = cantidad_total_bytes_serializado(instrucciones, tamanio_programa);
    void *buffer = malloc(bytes);

    int desplazamiento = 0;

    memcpy(buffer + desplazamiento, &tamanio_programa, sizeof(uint32_t));
    desplazamiento += sizeof(uint32_t);

    t_list_iterator *iterator = list_iterator_create(instrucciones);
    while (list_iterator_has_next(iterator))
    {
        t_instruccion *instruccion = (t_instruccion *)list_iterator_next(iterator);

        uint32_t tamanio_codigo = strlen(instruccion->codigo_instruccion) + 1;
        memcpy(buffer + desplazamiento, &tamanio_codigo, sizeof(uint32_t));
        desplazamiento += sizeof(uint32_t);
        memcpy(buffer + desplazamiento, &(instruccion->codigo_instruccion), tamanio_codigo);
        desplazamiento += tamanio_codigo;

        uint32_t cant_parametros = instruccion->cantidad_parametros;
        memcpy(buffer + desplazamiento, &cant_parametros, sizeof(uint32_t));
        desplazamiento += sizeof(uint32_t);
        for (int i = 0; i < cant_parametros; i++)
        {
            uint32_t parametro = instruccion->parametros[i];
            memcpy(buffer + desplazamiento, &parametro, sizeof(uint32_t));
            desplazamiento += sizeof(uint32_t);
        }
    }

    list_iterator_destroy(iterator);

    return buffer;
}

int cantidad_total_bytes_serializado(t_list *instrucciones, uint32_t tamanio_programa)
{
    /*     
        TAM_PROGRAMA (uint32), { TAM_CODIGO (uint32), CODIGO (TAM_CODIGO), CANT_PARAM (uint32)[, PARAM1 (uint32)[, PARAM2(uint32)]] }
    */
    int bytes = 0;

    bytes += sizeof(tamanio_programa);

    t_list_iterator *iterator = list_iterator_create(instrucciones);
    while (list_iterator_has_next(iterator))
    {
        t_instruccion *instruccion = (t_instruccion *)list_iterator_next(iterator);

        uint32_t tamanio_codigo = strlen(instruccion->codigo_instruccion) + 1;
        bytes += sizeof(tamanio_codigo);
        bytes += tamanio_codigo;

        uint32_t cant_parametros = instruccion->cantidad_parametros;
        bytes += sizeof(cant_parametros);                   // Cantidad de parametros
        bytes += sizeof(cant_parametros) * cant_parametros; // Cada parametro
    }

    list_iterator_destroy(iterator);

    return bytes;
}