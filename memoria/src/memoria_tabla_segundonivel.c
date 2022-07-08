#include <memoria_tabla_segundonivel.h>

t_tabla_segundonivel *tabla_segundonivel_new(uint32_t pid, uint32_t numero_tabla_segundonivel, uint32_t posicion_tabla_segundonivel_en_proceso)
{
    t_tabla_segundonivel *tabla_segundonivel = malloc(sizeof(t_tabla_segundonivel));

    tabla_segundonivel->pid = pid;
    tabla_segundonivel->numero_tabla_segundonivel = numero_tabla_segundonivel;

    tabla_segundonivel->entradas_segundonivel = list_create();
    for (int i = 0; i < config->entradas_por_tabla; i++)
    {
        uint32_t numero_entrada = i;

        /* Para la primera tabla de 2N la posicion_tabla es 0, asi que las paginas van a estar en [0, entradas_por_tabla - 1].
         * Para la segunda tabla de 2N la posicion es 1, asi que las paginas van a estar en [entradas_por_tabla, 2*entradas_por_tabla - 1]
         */
        uint32_t numero_pagina = posicion_tabla_segundonivel_en_proceso * config->entradas_por_tabla + i;

        t_entrada_segundonivel *entrada_segundonivel = entrada_segundonivel_new(pid, numero_entrada, numero_pagina);
        list_add(tabla_segundonivel->entradas_segundonivel, entrada_segundonivel);
    }

    return tabla_segundonivel;
}

void tabla_segundonivel_destroy(t_tabla_segundonivel *tabla_segundonivel)
{
    list_destroy_and_destroy_elements(tabla_segundonivel->entradas_segundonivel, (void *)entrada_segundonivel_destroy);
    free(tabla_segundonivel->entradas_segundonivel);

    free(tabla_segundonivel);
}

t_entrada_segundonivel *tabla_segundonivel_obtener_entrada_segundo_nivel(t_tabla_segundonivel *self, uint32_t entrada_tablasegundonivel)
{
    t_entrada_segundonivel *entrada_buscada = NULL;

    t_list_iterator *iterator = list_iterator_create(self->entradas_segundonivel);
    while (list_iterator_has_next(iterator) && entrada_buscada == NULL)
    {
        t_entrada_segundonivel *entrada = list_iterator_next(iterator);
        if (entrada->numero_entrada == entrada_tablasegundonivel)
        {
            entrada_buscada = entrada;
        }
    }
    list_iterator_destroy(iterator);

    return entrada_buscada;
}

t_tabla_segundonivel *obtener_tablasegundonivel_por_numero(t_list *tablas_segundonivel, uint32_t numero_tablasegundonivel)
{
    bool buscar_tablasegundonivel_por_numero(void *element)
    {
        t_tabla_segundonivel *tablasegundonivel = element;
        return tablasegundonivel->numero_tabla_segundonivel == numero_tablasegundonivel;
    }

    return list_find(tablas_segundonivel, buscar_tablasegundonivel_por_numero);
}
