#ifndef MEMORIA_RAM_H
#define MEMORIA_RAM_H

#include <memoria_utils.h>
#include <memoria_tabla_paginas_primernivel.h>

#include <stdlib.h>
#include <commons/collections/list.h>

typedef struct t_memoria_ram
{
    void *memoria_usuario;
    t_list *tablas_paginas_primer_nivel;
    // t_list *archivos_swap;
} t_memoria_ram;

t_memoria_ram *memoria_ram_new();
void memoria_ram_destroy(t_memoria_ram *memoria);

#endif
