#ifndef MEMORIA_TABLA_PAGINAS_PRIMERNIVEL_H
#define MEMORIA_TABLA_PAGINAS_PRIMERNIVEL_H

#include <memoria_utils.h>
#include <memoria_tabla_paginas_segundonivel.h>

#include <stdint.h>
#include <stdlib.h>
#include <commons/collections/list.h>

typedef struct t_tabla_paginas_primernivel
{
    uint32_t numero_tabla;
    uint32_t pid;
    t_list *tablas_paginas_segundonivel;
} t_tabla_paginas_primernivel;

t_tabla_paginas_primernivel *tabla_paginas_primernivel_new(uint32_t pid, uint32_t tamanio_proceso);
void tabla_paginas_primernivel_destroy(t_tabla_paginas_primernivel *tabla_primernivel);
void tabla_paginas_primernivel_destroy_aux(void *tabla_primernivel);

#endif
