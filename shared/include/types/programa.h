#ifndef TYPES_PROGRAMA_H
#define TYPES_PROGRAMA_H

#include <types/instruccion.h>
#include <serialization/serialization_utils.h>
#include <utils/logs.h>
#include <utils/pointers.h>
#include <utils/string.h>

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/collections/list.h>
#include <commons/log.h>
#include <commons/string.h>

typedef struct t_programa
{
    char *ruta;
    uint32_t tamanio;
    t_list *instrucciones; // De tipo t_instruccion
} t_programa;

/**
 * @brief Crea un t_programa en base al archivo de la ruta especificada
 */
t_programa *programa_new(char *ruta, uint32_t tamanio, t_log *logger, pthread_mutex_t *mutex_logger);

/**
 * @brief Crea un t_programa usando las instrucciones pasadas por parametro
 */
t_programa *programa_new_with_instructions(t_list *instrucciones, uint32_t tamanio);

/**
 * @brief Devuelve la cantidad de instrucciones que tiene un programa
 */
uint32_t programa_cantidad_instrucciones(t_programa *programa);

/**
 * @brief Libera los recursos del t_programa
 */
void programa_destroy(t_programa *programa);

/**
 * @brief Crea una lista de instrucciones en base al archivo de la ruta especificada
 */
t_list *crear_instrucciones_de_archivo(char *ruta, t_log *logger, pthread_mutex_t *mutex_logger);

void *serializar_programa(t_programa *programa, int *bytes);
t_programa *deserializar_programa(void *buffer);
int bytes_totales_programa_serializado(t_programa *programa);

#endif
