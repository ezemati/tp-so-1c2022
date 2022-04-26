#ifndef MEMORIA_TABLA_PAGINAS_SEGUNDONIVEL_H
#define MEMORIA_TABLA_PAGINAS_SEGUNDONIVEL_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct t_tabla_paginas_segundonivel
{
    // uint32_t numero_tabla;
    uint32_t numero_marco;
    bool bit_presencia;
    bool bit_uso;
    bool bit_modificado;
} t_tabla_paginas_segundonivel;

t_tabla_paginas_segundonivel *tabla_paginas_segundonivel_new();
void tabla_paginas_segundonivel_destroy(t_tabla_paginas_segundonivel *tabla_segundonivel);
void tabla_paginas_segundonivel_destroy_aux(void *tabla_segundonivel);

#endif