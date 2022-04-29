#ifndef MEMORIA_ENTRADA_SEGUNDONIVEL_H
#define MEMORIA_ENTRADA_SEGUNDONIVEL_H

#include <commons/collections/list.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct t_entrada_segundonivel
{
    uint32_t numero_entrada; // Posicion de la entrada dentro de la tabla de 2N
    uint32_t numero_marco;
    bool bit_presencia;
    bool bit_uso;
    bool bit_modificado;
} t_entrada_segundonivel;

t_entrada_segundonivel *entrada_segundonivel_new(uint32_t numero_entrada);
void entrada_segundonivel_destroy(t_entrada_segundonivel *entrada_segundonivel);
bool entrada_segundonivel_tiene_pagina_presente(t_entrada_segundonivel *self);
uint32_t obtener_marco_de_pagina_de_entrada_segundonivel(t_entrada_segundonivel *self);

#endif
