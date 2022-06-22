#ifndef MEMORIA_CONFIG_H
#define MEMORIA_CONFIG_H

#include <types/time_types.h>
#include <utils/logs.h>

#include <stdint.h>
#include <stdlib.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/string.h>

typedef struct t_memoria_config
{
    uint32_t puerto_escucha;
    uint32_t tamanio_memoria;
    uint32_t tamanio_pagina;
    uint32_t entradas_por_tabla;
    time_miliseg retardo_memoria;
    char *algoritmo_reemplazo;
    uint32_t marcos_por_proceso;
    time_miliseg retardo_swap;
    char *path_swap;
} t_memoria_config;

/**
 * @brief Crea un puntero a una estructura t_memoria_config
 */
t_memoria_config *memoria_config_new(char *config_path);

/**
 * @brief Libera los recursos de un t_memoria_config
 */
void memoria_config_destroy(t_memoria_config *config);

#endif
