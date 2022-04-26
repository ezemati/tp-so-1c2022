#include <memoria_tabla_paginas_primernivel.h>

t_tabla_paginas_primernivel *tabla_paginas_primernivel_new(uint32_t pid, uint32_t tamanio_proceso)
{
    t_tabla_paginas_primernivel *tabla_primernivel = malloc(sizeof(t_tabla_paginas_primernivel));
    tabla_primernivel->pid = pid;
    tabla_primernivel->numero_tabla = pid; // En principio el numero de la tabla de 1N es igual al PID, ver si habria que sacarlo

    tabla_primernivel->tablas_paginas_segundonivel = list_create(); // TODO: crear las tablas de 2N necesarias
}

void tabla_paginas_primernivel_destroy(t_tabla_paginas_primernivel *tabla_primernivel)
{
    list_destroy_and_destroy_elements(tabla_primernivel->tablas_paginas_segundonivel, tabla_paginas_segundonivel_destroy_aux);
    free(tabla_primernivel->tablas_paginas_segundonivel);
    free(tabla_primernivel);
}

void tabla_paginas_primernivel_destroy_aux(void *tabla_primernivel)
{
    tabla_paginas_primernivel_destroy((t_tabla_paginas_primernivel *)tabla_primernivel);
}
