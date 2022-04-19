#ifndef SHARED_INSTRUCCION_H
#define SHARED_INSTRUCCION_H

#include <stdint.h>
#include <stdlib.h>
#include <commons/string.h>

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
 * @DESC: Libera los recursos de una t_instruccion
 */
void instruccion_destroy(t_instruccion *instruccion);

#endif