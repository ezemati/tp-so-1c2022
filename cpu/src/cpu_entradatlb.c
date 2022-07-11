#include <cpu_entradatlb.h>

t_cpu_entradatlb *entradatlb_new()
{
    t_cpu_entradatlb *entradatlb = malloc(sizeof(t_cpu_entradatlb));
    entradatlb->numero_pagina = -1;
    entradatlb->numero_marco = -1;
    entradatlb->last_used = 0;
    return entradatlb;
}

void entradatlb_destroy(t_cpu_entradatlb *entrada)
{
    free(entrada);
}

void entradatlb_update(t_cpu_entradatlb *entrada, uint32_t numero_pagina, uint32_t numero_marco)
{
    entrada->numero_pagina = numero_pagina;
    entrada->numero_marco = numero_marco;
    entradatlb_marcar_usada(entrada);
}

void entradatlb_clear(t_cpu_entradatlb *entrada)
{
    entrada->numero_pagina = -1;
    entrada->numero_marco = -1;
    entrada->last_used = 0;
}

void entradatlb_marcar_usada(t_cpu_entradatlb *entrada)
{
    entrada->last_used = current_time();
}
