#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <commons/collections/list.h>

typedef struct t_instruccion
{
    char *codigo_instruccion;
    int cantidad_parametros;
    uint32_t parametros[2];
} t_instruccion;

void *serializar_instrucciones(t_list *instrucciones, uint32_t tamanio_programa, int *bytes);
int cantidad_total_bytes_serializado(t_list *instrucciones, uint32_t tamanio_programa);

#endif
