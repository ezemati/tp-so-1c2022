#include <memoria_tabla_paginas_segundonivel.h>

t_tabla_paginas_segundonivel *tabla_paginas_segundonivel_new()
{
    t_tabla_paginas_segundonivel *tabla_segundonivel = malloc(sizeof(t_tabla_paginas_segundonivel));
    tabla_segundonivel->bit_presencia = false;
    tabla_segundonivel->bit_modificado = false;
    tabla_segundonivel->bit_uso = true;
    tabla_segundonivel->numero_marco = -1;
    return tabla_segundonivel;
}

void tabla_paginas_segundonivel_destroy(t_tabla_paginas_segundonivel *tabla_segundonivel)
{
    free(tabla_segundonivel);
}

void tabla_paginas_segundonivel_destroy_aux(void *tabla_segundonivel)
{
    tabla_paginas_segundonivel_destroy((t_tabla_paginas_segundonivel *)tabla_segundonivel);
}
