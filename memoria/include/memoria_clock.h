#ifndef MEMORIA_CLOCK_H
#define MEMORIA_CLOCK_H

#include <memoria_config.h>
#include <memoria_entrada_segundonivel.h>
#include <memoria_variables_globales.h>

#include <stdbool.h>
#include <stdint.h>
#include <commons/collections/list.h>
#include <commons/string.h>

typedef enum algoritmo_reemplazo
{
    CLOCK = 1,
    CLOCK_M
} algoritmo_reemplazo;

// Un clock para cada proceso
typedef struct t_clock
{
    t_list *entradas_segundonivel; // De tipo t_entrada_segundonivel
    algoritmo_reemplazo algor_reemplazo;
    uint32_t puntero_aguja;
} t_clock;

t_clock *clock_new(char *algor_reemplazo);
void clock_destroy(t_clock *clock);
void clock_clear(t_clock *clock);

uint32_t clock_obtener_posicion_pagina_a_reemplazar(t_clock *self);
void clock_agregar_entrada(t_clock *self, t_entrada_segundonivel *entradaNueva);
void clock_reemplazar_entrada(t_clock *self, t_entrada_segundonivel *entradaNueva, uint32_t posicion);
t_entrada_segundonivel *clock_obtener_entrada_en_posicion(t_clock *self, uint32_t index);
bool clock_esta_lleno(t_clock *self);
uint32_t clock_cantidad_entradas_llenas(t_clock *self);

#endif
