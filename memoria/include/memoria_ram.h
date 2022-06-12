#ifndef MEMORIA_RAM_H
#define MEMORIA_RAM_H

#include <memoria_tabla_primernivel.h>
#include <memoria_tabla_segundonivel.h>
#include <memoria_swap.h>
#include <memoria_variables_globales.h>

#include <memoria/leer_dato.h>
#include <memoria/escribir_dato.h>
#include <memoria/numero_tabla_2_para_entrada_tabla_1.h>
#include <memoria/marco_para_entrada_tabla_2.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/collections/list.h>
#include <commons/string.h>

typedef struct t_memoria_ram
{
    void *memoria_usuario;
    bool *bitmap_marcos_libres;   // true significa libre, false significa ocupado (TODO: invertir significado del true y false)
    t_list *tablas_primer_nivel;  // De tipo t_tabla_primernivel, hay una por proceso
    t_list *tablas_segundo_nivel; // De tipo t_tabla_segundonivel
} t_memoria_ram;

t_memoria_ram *memoria_ram_new();

void memoria_ram_destroy(t_memoria_ram *memoria);

/**
 * @brief Inicializa las estructuras necesarias y devuelve el numero de la tabla de 1er nivel del nuevo proceso
 */
uint32_t memoria_ram_agregar_proceso(t_memoria_ram *self, uint32_t pid, uint32_t tamanio_proceso);

/**
 * @brief Saca todas las paginas del proceso de la RAM y las guarda en el archivo SWAP
 */
void memoria_ram_suspender_proceso(t_memoria_ram *self, uint32_t pid, uint32_t numero_tablaprimernivel);

/**
 * @brief Libera todos los recursos del proceso (incluyendo SWAP)
 */
void memoria_ram_finalizar_proceso(t_memoria_ram *self, uint32_t numero_tablaprimernivel);

/**
 * @brief Lee los bytes especificados de la direccion fisica especificada
 */
void *memoria_ram_leer_dato(t_memoria_ram *self, t_memoria_leerdato_request *request);

/**
 * @brief Escribe los bytes especificados en la direccion fisica especificada
 */
void memoria_ram_escribir_dato(t_memoria_ram *self, t_memoria_escribirdato_request *request);

uint32_t memoria_ram_obtener_numero_tabla_2_para_entrada_tabla_1(t_memoria_ram *self, t_memoria_numerotabla2paraentradatabla1_request *request);

uint32_t memoria_ram_obtener_numero_marco_para_entrada_tabla_2(t_memoria_ram *self, t_memoria_marcoparaentradatabla2_request *request);

void memoria_ram_cargar_pagina(t_memoria_ram *self, t_entrada_segundonivel *entradaNueva, t_clock *clock);
void memoria_ram_reemplazar_pagina(t_memoria_ram *self, uint32_t pid, t_entrada_segundonivel *entradaNueva, t_entrada_segundonivel *entradaVieja);
void memoria_ram_vaciar_marco(t_memoria_ram *self, uint32_t numero_marco);
int32_t memoria_ram_obtener_numero_marco_libre(t_memoria_ram *self);
void memoria_ram_marcar_marco_ocupado(t_memoria_ram *self, uint32_t numero_marco);
void memoria_ram_marcar_marco_libre(t_memoria_ram *self, uint32_t numero_marco);
uint32_t memoria_ram_obtener_cantidad_marcos_totales();
bool memoria_ram_marco_esta_libre(t_memoria_ram *self, uint32_t numero_marco);
t_entrada_segundonivel *memoria_ram_obtener_pagina_en_marco(t_memoria_ram *self, uint32_t numero_marco);

#endif
