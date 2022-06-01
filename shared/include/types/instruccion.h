#ifndef TYPES_INSTRUCCION_H
#define TYPES_INSTRUCCION_H

#include <serialization/serialization_utils.h>
#include <utils/string.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <commons/string.h>
#include <commons/collections/list.h>

typedef enum code_instruccion
{
    NO_OP = 1,
    IO,
    READ,
    WRITE,
    COPY,
    EXIT
} code_instruccion;

typedef struct t_instruccion
{
    code_instruccion codigo_instruccion;
    int cantidad_parametros;
    uint32_t parametros[2];
} t_instruccion;

/**
 * @brief Crea un puntero a una estructura t_instruccion
 */
t_instruccion *instruccion_new(char *codigo_instruccion, char **parametros);

/**
 * @brief Crea un puntero a una estructura t_instruccion, usando los parametros numericos pasados por parametro
 */
t_instruccion *instruccion_new_with_numeric_params(code_instruccion codigo_instruccion, uint32_t cant_parametros, uint32_t *parametros);

/**
 * @brief Duplica una instancia de t_instruccion
 */
t_instruccion *instruccion_duplicate(t_instruccion *instruccion);

/**
 * @brief Duplica una lista de instrucciones, duplicando a cada instruccion
 */
t_list *instrucciones_duplicate(t_list *lista_instrucciones);

/**
 * @brief Libera los recursos de una t_instruccion
 */
void instruccion_destroy(t_instruccion *instruccion);

/**
 * @brief Libera los recursos de todas las t_instruccion de una lista
 */
void instrucciones_destroy(t_list *instrucciones);

char *format_instruccion_para_print(t_instruccion *instruccion);

void *serializar_instruccion(t_instruccion *instruccion, int *bytes);
t_instruccion *deserializar_instruccion(void *buffer, int *desplazamiento);
int bytes_totales_instruccion_serializada(t_instruccion *instruccion);

void *serializar_lista_instrucciones(t_list *lista_instrucciones, int *bytes);
void serializar_lista_instrucciones_en_buffer(t_list *lista_instrucciones, void *buffer, int *desplazamiento);
t_list *deserializar_lista_instrucciones(void *buffer, int *desplazamiento);
int bytes_totales_lista_instrucciones_serializada(t_list *lista_instrucciones);

#endif