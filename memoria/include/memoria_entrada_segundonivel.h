#ifndef MEMORIA_ENTRADA_SEGUNDONIVEL_H
#define MEMORIA_ENTRADA_SEGUNDONIVEL_H

#include <memoria_variables_globales.h>
#include <utils/logs.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <commons/collections/list.h>

typedef struct t_entrada_segundonivel
{
    uint32_t pid;
    uint32_t numero_entrada; // Posicion de la entrada dentro de la tabla de 2N
    uint32_t numero_pagina;
    int32_t numero_marco;
    bool bit_presencia;
    bool bit_uso;
    bool bit_modificado;
} t_entrada_segundonivel;

t_entrada_segundonivel *entrada_segundonivel_new(uint32_t pid, uint32_t numero_entrada, uint32_t numero_pagina);
void entrada_segundonivel_destroy(t_entrada_segundonivel *entrada_segundonivel);
bool entrada_segundonivel_tiene_pagina_presente(t_entrada_segundonivel *self);
uint32_t marco_de_pagina_de_entrada_segundonivel(t_entrada_segundonivel *self);
void entrada_segundonivel_marcar_pagina_cargada(t_entrada_segundonivel *self, uint32_t numero_marco);
void entrada_segundonivel_marcar_pagina_descargada(t_entrada_segundonivel *self);

bool pagina_esta_modificada(t_entrada_segundonivel *self);
void marcar_pagina_modificada(t_entrada_segundonivel *self);
void marcar_pagina_usada(t_entrada_segundonivel *self);

#endif
