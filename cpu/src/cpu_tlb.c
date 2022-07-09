#include <cpu_tlb.h>

static bool tlb_tiene_espacios_libres(t_cpu_tlb *self);
static void tlb_add_entry_in_empty_space(t_cpu_tlb *self, uint32_t numero_pagina, uint32_t numero_marco);
static t_cpu_entradatlb *tlb_get_entry_to_replace_lru(t_cpu_tlb *self);
static t_cpu_entradatlb *tlb_get_entry_to_replace_fifo(t_cpu_tlb *self);

t_cpu_tlb *tlb_new(uint32_t cantidad_entradas_totales)
{
    t_cpu_tlb *self = malloc(sizeof(t_cpu_tlb));
    self->cantidad_entradas_totales = cantidad_entradas_totales;
    self->cantidad_entradas_ocupadas = 0;
    self->indice_reemplazo_fifo = 0;

    self->entradas_tlb = list_create();
    for (int i = 0; i < self->cantidad_entradas_totales; i++)
    {
        t_cpu_entradatlb *entrada_tlb = entradatlb_new();
        list_add(self->entradas_tlb, entrada_tlb);
    }

    return self;
}

void tlb_destroy(t_cpu_tlb *self)
{
    list_destroy_and_destroy_elements(self->entradas_tlb, (void *)entradatlb_destroy);

    free(self);
}

void tlb_clear(t_cpu_tlb *self)
{
    for (int i = 0; i < self->cantidad_entradas_ocupadas; i++)
    {
        t_cpu_entradatlb *entrada = list_get(self->entradas_tlb, i);
        entradatlb_clear(entrada);
    }

    self->cantidad_entradas_ocupadas = 0;
}

void tlb_add_entry(t_cpu_tlb *self, uint32_t numero_pagina, uint32_t numero_marco)
{
    // TODO: printear en trace todas las entradas de la TLB en forma [pagina, marco]
    // (similar al print en la planificacion del Kernel)

    if (tlb_tiene_espacios_libres(self))
    {
        log_info_with_mutex(logger, &mutex_logger, "TLB: agregando pagina %d en espacio vacio", numero_pagina);
        tlb_add_entry_in_empty_space(self, numero_pagina, numero_marco);
        return;
    }

    t_cpu_entradatlb *entrada_a_reemplazar = string_equals_ignore_case(config->reemplazo_tlb, "FIFO")
                                                 ? tlb_get_entry_to_replace_fifo(self)
                                                 : tlb_get_entry_to_replace_lru(self);

    log_info_with_mutex(logger, &mutex_logger, "TLB: haciendo reemplazo, sale pagina %d y entra pagina %d", entrada_a_reemplazar->numero_pagina, numero_pagina);

    entradatlb_update(entrada_a_reemplazar, numero_pagina, numero_marco);
}

void tlb_replace_entry(t_cpu_tlb *self, t_cpu_entradatlb *entrada, uint32_t numero_pagina_nueva)
{
    uint32_t numero_marco = entrada->numero_marco;
    entradatlb_update(entrada, numero_pagina_nueva, numero_marco);
}

int tlb_try_read_entry(t_cpu_tlb *self, uint32_t numero_pagina)
{
    for (int i = 0; i < self->cantidad_entradas_ocupadas; i++)
    {
        t_cpu_entradatlb *entrada = list_get(self->entradas_tlb, i);
        if (entrada->numero_pagina == numero_pagina)
        {
            entradatlb_marcar_usada(entrada);
            return entrada->numero_marco;
        }
    }

    return -1;
}

t_cpu_entradatlb *tlb_get_entry_con_numero_pagina(t_cpu_tlb *self, uint32_t numero_pagina)
{
    t_cpu_entradatlb *entrada_con_pagina = NULL;

    t_list_iterator *iterator = list_iterator_create(self->entradas_tlb);
    while (list_iterator_has_next(iterator) && entrada_con_pagina == NULL)
    {
        t_cpu_entradatlb *entrada = list_iterator_next(iterator);
        if (entrada->numero_pagina == numero_pagina)
        {
            entrada_con_pagina = entrada;
        }
    }
    list_iterator_destroy(iterator);

    return entrada_con_pagina;
}

static bool tlb_tiene_espacios_libres(t_cpu_tlb *self)
{
    return self->cantidad_entradas_ocupadas < self->cantidad_entradas_totales;
}

static void tlb_add_entry_in_empty_space(t_cpu_tlb *self, uint32_t numero_pagina, uint32_t numero_marco)
{
    // Si tengo 0 entradas ocupadas, significa que la proxima entrada la tengo que guardar en el indice 0
    uint32_t empty_index = self->cantidad_entradas_ocupadas;

    t_cpu_entradatlb *entrada = list_get(self->entradas_tlb, empty_index);

    entradatlb_update(entrada, numero_pagina, numero_marco);

    self->cantidad_entradas_ocupadas++;
}

static t_cpu_entradatlb *tlb_get_entry_to_replace_fifo(t_cpu_tlb *self)
{
    uint32_t index_to_replace = self->indice_reemplazo_fifo;
    t_cpu_entradatlb *entrada = list_get(self->entradas_tlb, index_to_replace);
    self->indice_reemplazo_fifo++;
    return entrada;
}

static t_cpu_entradatlb *tlb_get_entry_to_replace_lru(t_cpu_tlb *self)
{
    t_cpu_entradatlb *get_least_recently_used(t_cpu_entradatlb * entry1, t_cpu_entradatlb * entry2)
    {
        // Si entry1 se uso despues que entry2, entonces el LRU es entry2
        return comparar_times(entry1->last_used, entry2->last_used) > 0
                   ? entry2
                   : entry1;
    }

    return list_get_minimum(self->entradas_tlb, (void *)get_least_recently_used);
}
