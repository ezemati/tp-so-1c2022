#include <memoria_tabla_primernivel.h>

t_tabla_primernivel *tabla_primernivel_new(uint32_t pid, uint32_t tamanio_proceso, uint32_t numero_tablaprimernivel, t_list *lista_tablas_segundonivel)
{
    t_tabla_primernivel *tabla_primernivel = malloc(sizeof(t_tabla_primernivel));
    tabla_primernivel->pid = pid;
    tabla_primernivel->numero_tablaprimernivel = numero_tablaprimernivel;
    tabla_primernivel->tamanio_proceso = tamanio_proceso;
    tabla_primernivel->clock = clock_new(config->algoritmo_reemplazo);

    // Si esta es la tabla de 1N 0, entonces hay 0 tablas de 2N creadas
    // Si esta es la tabla de 1N 1, entonces hay entradas_por_tabla tablas de 2N creadas
    int cantidad_total_tablas_segundonivel = numero_tablaprimernivel * config->entradas_por_tabla;
    tabla_primernivel->numeros_tablas_segundonivel = malloc(sizeof(uint32_t) * config->entradas_por_tabla);
    for (int i = 0; i < config->entradas_por_tabla; i++)
    {
        uint32_t numero_tabla_segundonivel = cantidad_total_tablas_segundonivel + i;
        t_tabla_segundonivel *tabla_segundonivel = tabla_segundonivel_new(pid, numero_tabla_segundonivel, i);
        list_add(lista_tablas_segundonivel, tabla_segundonivel);

        tabla_primernivel->numeros_tablas_segundonivel[i] = numero_tabla_segundonivel;
    }

    return tabla_primernivel;
}

void tabla_primernivel_destroy(t_tabla_primernivel *tabla_primernivel)
{
    free(tabla_primernivel->numeros_tablas_segundonivel);
    clock_destroy(tabla_primernivel->clock);
    free(tabla_primernivel);
}

uint32_t tabla_primernivel_obtener_numero_tabla_segundo_nivel_en_entrada(t_tabla_primernivel *self, t_list *tablas_segundo_nivel, uint32_t entrada_tablaprimernivel)
{
    uint32_t numero_tablasegundonivel = self->numeros_tablas_segundonivel[entrada_tablaprimernivel];
    t_tabla_segundonivel *tablasegundonivel = obtener_tablasegundonivel_por_numero(tablas_segundo_nivel, numero_tablasegundonivel);
    return tablasegundonivel->numero_tabla_segundonivel;
}

t_tabla_primernivel *obtener_tablaprimernivel_por_numero(t_list *tablas_primernivel, uint32_t numero_tablaprimernivel)
{
    bool buscar_tablaprimernivel_por_numero(void *element)
    {
        t_tabla_primernivel *tablaprimernivel = element;
        return tablaprimernivel->numero_tablaprimernivel == numero_tablaprimernivel;
    }

    return list_find(tablas_primernivel, buscar_tablaprimernivel_por_numero);
}
