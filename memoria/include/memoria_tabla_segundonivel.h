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
    uint32_t pid;
    uint32_t numero_tabla_segundonivel;
    t_list *entradas_segundonivel; // De tipo t_entrada_segundonivel
} t_tabla_segundonivel;

/**
 * @param numero_tabla_segundonivel El numero global de la tabla de 2N (si por ejemplo ya hay 10 tablas de 2N de otros procesos, este numero va a ser 11, despues 12...)
 * @param posicion_tabla_segundonivel_en_proceso La posicion de esta tabla de 2N dentro de este proceso en particular (0, 1, 2 ..., sin importar los demas procesos)
 */
t_tabla_segundonivel *tabla_segundonivel_new(uint32_t pid, uint32_t numero_tabla_segundonivel, uint32_t posicion_tabla_segundonivel_en_proceso);

void tabla_segundonivel_destroy(t_tabla_segundonivel *tabla_segundonivel);

t_entrada_segundonivel *tabla_segundonivel_obtener_entrada_segundo_nivel(t_tabla_segundonivel *self, uint32_t entrada_tablasegundonivel);

t_tabla_segundonivel *obtener_tablasegundonivel_por_numero(t_list *tablas_segundonivel, uint32_t numero_tablasegundonivel);
t_tabla_segundonivel *remover_tablasegundonivel_por_numero(t_list *tablas_segundonivel, uint32_t numero_tablasegundonivel);

#endif
