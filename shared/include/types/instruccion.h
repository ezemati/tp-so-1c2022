#ifndef TYPES_INSTRUCCION_H
#define TYPES_INSTRUCCION_H

#include <stdint.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/collections/list.h>

typedef struct t_instruccion
{
    char *codigo_instruccion;
    int cantidad_parametros;
    uint32_t parametros[2];
} t_instruccion;

/**
 * @DESC: Crea un puntero a una estructura t_instruccion
 */
t_instruccion *instruccion_new(char *codigo_instruccion, char **parametros);

/**
 * @DESC: Crea un puntero a una estructura t_instruccion, usando los parametros numericos pasados por parametro
 */
t_instruccion *instruccion_new_with_numeric_params(char *codigo_instruccion, uint32_t cant_parametros, uint32_t *parametros);

/**
 * @DESC: Libera los recursos de una t_instruccion
 */
void instruccion_destroy(t_instruccion *instruccion);

/**
 * @DESC: Libera los recursos de todas las t_instruccion de una lista
 */
void instrucciones_destroy(t_list *instrucciones);

#endif