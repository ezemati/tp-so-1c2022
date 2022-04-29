#include <memoria_entrada_segundonivel.h>

t_entrada_segundonivel *entrada_segundonivel_new(uint32_t numero_entrada)
{
    t_entrada_segundonivel *entrada_segundonivel = malloc(sizeof(t_entrada_segundonivel));
    entrada_segundonivel->numero_entrada = numero_entrada;
    entrada_segundonivel->numero_marco = -1;
    entrada_segundonivel->bit_presencia = false;
    entrada_segundonivel->bit_modificado = false;
    entrada_segundonivel->bit_uso = true;
    return entrada_segundonivel;
}

void entrada_segundonivel_destroy(t_entrada_segundonivel *entrada_segundonivel)
{
    free(entrada_segundonivel);
}

bool entrada_segundonivel_tiene_pagina_presente(t_entrada_segundonivel *entrada_segundonivel)
{
    return entrada_segundonivel->bit_presencia;
}

uint32_t obtener_marco_de_pagina_de_entrada_segundonivel(t_entrada_segundonivel *self)
{
    return self->numero_marco;
}
