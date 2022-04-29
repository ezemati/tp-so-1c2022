#ifndef TYPES_PROGRAMA_H
#define TYPES_PROGRAMA_H

#include <types/instruccion.h>
#include <serialization/serialization_utils.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/collections/list.h>
#include <commons/string.h>

typedef struct t_programa
{
    char *ruta;
    uint32_t tamanio;
    t_list *instrucciones;
} t_programa;

/**
 * @DESC: Crea un t_programa en base al archivo de la ruta especificada
 */
t_programa *programa_new(char *ruta, uint32_t tamanio);

/**
 * @DESC: Crea un t_programa usando las instrucciones pasadas por parametro
 */
t_programa *programa_new_with_instructions(t_list *instrucciones, uint32_t tamanio);

/**
 * @DESC: Devuelve la cantidad de instrucciones que tiene un programa
 */
uint32_t programa_cantidad_instrucciones(t_programa *programa);

/**
 * @DESC: Libera los recursos del t_programa
 */
void programa_destroy(t_programa *programa);

/**
 * @DESC: Crea una lista de instrucciones en base al archivo de la ruta especificada
 */
t_list *crear_instrucciones_de_archivo(char *ruta);

void *serializar_programa(t_programa *programa, int *bytes);
int bytes_totales_programa_serializado(t_programa *programa);

#endif
