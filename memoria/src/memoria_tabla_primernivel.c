#include <memoria_tabla_primernivel.h>

t_tabla_primernivel *tabla_primernivel_new(uint32_t pid, uint32_t tamanio_proceso, uint32_t numero_tablaprimernivel, t_list *lista_tablas_segundonivel)
{
    t_tabla_primernivel *tabla_primernivel = malloc(sizeof(t_tabla_primernivel));
    tabla_primernivel->pid = pid;
    tabla_primernivel->numero_tablaprimernivel = numero_tablaprimernivel;
    tabla_primernivel->tamanio_proceso = tamanio_proceso;

    int total_segundonivel = list_size(lista_tablas_segundonivel);
    tabla_primernivel->indices_tablas_segundonivel = malloc(sizeof(uint32_t) * config->entradas_por_tabla);
    for (int i = 0; i < config->entradas_por_tabla; i++)
    {
        uint32_t index_lista_tablas_segundonivel = total_segundonivel + i;
        t_tabla_segundonivel *tabla_segundonivel = tabla_segundonivel_new(index_lista_tablas_segundonivel);
        list_add(lista_tablas_segundonivel, tabla_segundonivel);

        tabla_primernivel->indices_tablas_segundonivel[i] = index_lista_tablas_segundonivel;
    }

    return tabla_primernivel;
}

void tabla_primernivel_destroy(t_tabla_primernivel *tabla_primernivel)
{
    free(tabla_primernivel->indices_tablas_segundonivel);
    free(tabla_primernivel);
}
