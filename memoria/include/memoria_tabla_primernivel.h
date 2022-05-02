#ifndef MEMORIA_TABLA_PRIMERNIVEL_H
#define MEMORIA_TABLA_PRIMERNIVEL_H

#include <memoria_tabla_segundonivel.h>

#include <stdint.h>
#include <stdlib.h>
#include <commons/collections/list.h>

// Hay una TablaPrimerNivel por proceso
typedef struct t_tabla_primernivel
{
    uint32_t numero_tablaprimernivel;
    uint32_t pid;
    uint32_t tamanio_proceso;
    uint32_t *indices_tablas_segundonivel; // Los indices de las tablas de segundo nivel en la lista de la memoria_ram
} t_tabla_primernivel;

t_tabla_primernivel *tabla_primernivel_new(uint32_t pid, uint32_t tamanio_proceso, uint32_t numero_tablaprimernivel, t_list *lista_tablas_segundonivel);
void tabla_primernivel_destroy(t_tabla_primernivel *tabla_primernivel);

uint32_t tabla_primernivel_obtener_numero_tabla_segundo_nivel_en_entrada(t_tabla_primernivel *self, t_list *tablas_segundo_nivel, uint32_t entrada_tablaprimernivel);

#endif
