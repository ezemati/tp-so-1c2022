#include <memoria_tabla_primernivel.h>

t_tabla_primernivel *tabla_primernivel_new(uint32_t pid, uint32_t tamanio_proceso, uint32_t numero_tablaprimernivel, t_list *lista_tablas_segundonivel)
{
    t_tabla_primernivel *tabla_primernivel = malloc(sizeof(t_tabla_primernivel));
    tabla_primernivel->pid = pid;
    tabla_primernivel->numero_tablaprimernivel = numero_tablaprimernivel;
    tabla_primernivel->tamanio_proceso = tamanio_proceso;
    tabla_primernivel->clock = clock_new(config->algoritmo_reemplazo);

    int total_segundonivel = list_size(lista_tablas_segundonivel);
    tabla_primernivel->indices_tablas_segundonivel = malloc(sizeof(uint32_t) * config->entradas_por_tabla);
    for (int i = 0; i < config->entradas_por_tabla; i++)
    {
        uint32_t index_lista_tablas_segundonivel = total_segundonivel + i;
        t_tabla_segundonivel *tabla_segundonivel = tabla_segundonivel_new(pid, index_lista_tablas_segundonivel, i);
        list_add(lista_tablas_segundonivel, tabla_segundonivel);

        tabla_primernivel->indices_tablas_segundonivel[i] = index_lista_tablas_segundonivel;
    }

    return tabla_primernivel;
}

void tabla_primernivel_destroy(t_tabla_primernivel *tabla_primernivel)
{
    free(tabla_primernivel->indices_tablas_segundonivel);
    clock_destroy(tabla_primernivel->clock);
    free(tabla_primernivel);
}

uint32_t tabla_primernivel_obtener_numero_tabla_segundo_nivel_en_entrada(t_tabla_primernivel *self, t_list *tablas_segundo_nivel, uint32_t entrada_tablaprimernivel)
{
    uint32_t index_tablasegundonivel = self->indices_tablas_segundonivel[entrada_tablaprimernivel];
    t_entrada_segundonivel *entrada = list_get(tablas_segundo_nivel, index_tablasegundonivel);

    return entrada->numero_entrada;
}
