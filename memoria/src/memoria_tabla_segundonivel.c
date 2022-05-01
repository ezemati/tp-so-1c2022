#include <memoria_tabla_segundonivel.h>

t_tabla_segundonivel *tabla_segundonivel_new(uint32_t numero_tabla_segundonivel)
{
    t_tabla_segundonivel *tabla_segundonivel = malloc(sizeof(t_tabla_segundonivel));

    tabla_segundonivel->numero_tabla_segundonivel = numero_tabla_segundonivel;

    tabla_segundonivel->entradas_segundonivel = list_create();
    for (int i = 0; i < config->entradas_por_tabla; i++)
    {
        t_entrada_segundonivel *entrada_segundonivel = entrada_segundonivel_new(i);
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
