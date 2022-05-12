#ifndef MEMORIA_TABLA_SEGUNDONIVEL_H
#define MEMORIA_TABLA_SEGUNDONIVEL_H

#include <memoria_config.h>
#include <memoria_entrada_segundonivel.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <commons/collections/list.h>

typedef struct t_tabla_segundonivel
{
    uint32_t numero_tabla_segundonivel;
    t_list *entradas_segundonivel; // De tipo t_entrada_segundonivel
} t_tabla_segundonivel;

t_tabla_segundonivel *tabla_segundonivel_new(uint32_t numero_tablasegundonivel);
void tabla_segundonivel_destroy(t_tabla_segundonivel *tabla_segundonivel);

t_entrada_segundonivel *tabla_segundonivel_obtener_entrada_segundo_nivel(t_tabla_segundonivel *self, uint32_t entrada_tablasegundonivel);

#endif
