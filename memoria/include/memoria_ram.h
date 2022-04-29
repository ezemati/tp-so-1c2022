#ifndef MEMORIA_RAM_H
#define MEMORIA_RAM_H

#include <memoria_tabla_primernivel.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/collections/list.h>

typedef struct t_memoria_ram
{
    void *memoria_usuario;
    t_list *tablas_primer_nivel; // De tipo t_tabla_primernivel, hay una por proceso
    t_list *tablas_segundo_nivel; // De tipo t_tabla_segundonivel
} t_memoria_ram;

t_memoria_ram *memoria_ram_new();
void memoria_ram_destroy(t_memoria_ram *memoria);

/**
 * @DESC: Inicializa las estructuras necesarias y devuelve el numero de la tabla de 1er nivel del nuevo proceso
 */
uint32_t memoria_ram_agregar_proceso(t_memoria_ram *self, uint32_t pid, uint32_t tamanio_proceso);

/**
 * @DESC: libera todos los recursos del proceso (incluyendo SWAP)
 */
void memoria_ram_finalizar_proceso(t_memoria_ram *self, uint32_t numero_tablaprimernivel);

extern t_memoria_ram *memoria_ram;

#endif
