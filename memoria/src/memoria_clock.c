#include <memoria_clock.h>

static uint32_t clock_obtener_posicion_pagina_a_reemplazar_clock(t_clock *self);
static uint32_t clock_obtener_posicion_pagina_a_reemplazar_clockmodificado(t_clock *self);
static int32_t clock_buscar_entrada_con_uso0(t_clock *self);
static int32_t clock_buscar_entrada_con_uso0_modificado0(t_clock *self);
static int32_t clock_buscar_entrada_con_uso0_modificado1(t_clock *self);
static t_entrada_segundonivel *clock_obtener_entrada_actualmente_apuntada(t_clock *self);
static void clock_apuntar_a_siguiente(t_clock *self);

t_clock *clock_new(char *algor_reemplazo)
{
    t_clock *clock = malloc(sizeof(t_clock));
    clock->entradas_segundonivel = list_create();
    clock->algor_reemplazo = string_equals_ignore_case(algor_reemplazo, "CLOCK")
                                 ? CLOCK
                                 : CLOCK_M;
    clock->puntero_aguja = 0;
    return clock;
}

void clock_destroy(t_clock *clock)
{
    list_destroy(clock->entradas_segundonivel);
    free(clock);
}

void clock_clear(t_clock *clock)
{
    list_clean(clock->entradas_segundonivel);
    clock->puntero_aguja = 0;
}

uint32_t clock_obtener_posicion_pagina_a_reemplazar(t_clock *self)
{
    uint32_t indice_pagina_a_reemplazar;

    switch (self->algor_reemplazo)
    {
    case CLOCK:
        indice_pagina_a_reemplazar = clock_obtener_posicion_pagina_a_reemplazar_clock(self);
        break;
    case CLOCK_M:
        indice_pagina_a_reemplazar = clock_obtener_posicion_pagina_a_reemplazar_clockmodificado(self);
        break;
    default:
        indice_pagina_a_reemplazar = -1;
        break;
    }

    t_entrada_segundonivel *entrada_a_reemplazar = clock_obtener_entrada_en_posicion(self, indice_pagina_a_reemplazar);

    log_info_with_mutex(logger, &mutex_logger, "Entrada [%d] seleccionada para reemplazo: {pagina=%d}", indice_pagina_a_reemplazar, entrada_a_reemplazar->numero_pagina);

    return indice_pagina_a_reemplazar;
}

void clock_agregar_entrada(t_clock *self, t_entrada_segundonivel *entradaNueva)
{
    list_add(self->entradas_segundonivel, entradaNueva);
}

void clock_reemplazar_entrada(t_clock *self, t_entrada_segundonivel *entradaNueva, uint32_t posicion)
{
    list_replace(self->entradas_segundonivel, posicion, entradaNueva);
}

t_entrada_segundonivel *clock_obtener_entrada_en_posicion(t_clock *self, uint32_t index)
{
    return list_get(self->entradas_segundonivel, index);
}

bool clock_esta_lleno(t_clock *self)
{
    return clock_cantidad_entradas_llenas(self) >= config->marcos_por_proceso;
}

uint32_t clock_cantidad_entradas_llenas(t_clock *self)
{
    return list_size(self->entradas_segundonivel);
}

void clock_print_entradas(t_clock *self)
{
    for (int i = 0; i < config->marcos_por_proceso; i++)
    {
        t_entrada_segundonivel *entrada = clock_obtener_entrada_en_posicion(self, i);

        if (self->puntero_aguja == i)
        {
            if (self->algor_reemplazo == CLOCK)
                log_trace_with_mutex(logger, &mutex_logger, " --> [%d]: {pagina=%d, marco=%d} (U=%d)", i, entrada->numero_pagina, entrada->numero_marco, (int)entrada->bit_uso);
            else
                log_trace_with_mutex(logger, &mutex_logger, " --> [%d]: {pagina=%d, marco=%d} (U=%d, M=%d)", i, entrada->numero_pagina, entrada->numero_marco, (int)entrada->bit_uso, (int)entrada->bit_modificado);
        }
        else
        {
            if (self->algor_reemplazo == CLOCK)
                log_trace_with_mutex(logger, &mutex_logger, "     [%d]: {pagina=%d, marco=%d} (U=%d)", i, entrada->numero_pagina, entrada->numero_marco, (int)entrada->bit_uso);
            else
                log_trace_with_mutex(logger, &mutex_logger, "     [%d]: {pagina=%d, marco=%d} (U=%d, M=%d)", i, entrada->numero_pagina, entrada->numero_marco, (int)entrada->bit_uso, (int)entrada->bit_modificado);
        }
    }

    log_trace_with_mutex(logger, &mutex_logger, "     -----------------------------------------");
}

static uint32_t clock_obtener_posicion_pagina_a_reemplazar_clock(t_clock *self)
{
    return clock_buscar_entrada_con_uso0(self);
}

static uint32_t clock_obtener_posicion_pagina_a_reemplazar_clockmodificado(t_clock *self)
{
    int32_t posicionEntradaAReemplazar = -1;

    while (posicionEntradaAReemplazar == -1)
    {
        posicionEntradaAReemplazar = clock_buscar_entrada_con_uso0_modificado0(self);
        if (posicionEntradaAReemplazar == -1)
        {
            posicionEntradaAReemplazar = clock_buscar_entrada_con_uso0_modificado1(self);
        }
    }

    return posicionEntradaAReemplazar;
}

static int32_t clock_buscar_entrada_con_uso0(t_clock *self)
{
    int32_t posicionEntradaAReemplazar = -1;

    log_trace_with_mutex(logger, &mutex_logger, "Buscando entrada con (U=0)");

    while (posicionEntradaAReemplazar == -1)
    {
        clock_print_entradas(self);

        t_entrada_segundonivel *entradaActualmenteApuntada = clock_obtener_entrada_actualmente_apuntada(self);
        if (entradaActualmenteApuntada->bit_uso == false)
        {
            posicionEntradaAReemplazar = self->puntero_aguja;
        }
        else
        {
            entradaActualmenteApuntada->bit_uso = false;
        }

        clock_apuntar_a_siguiente(self);
    }

    return posicionEntradaAReemplazar;
}

static int32_t clock_buscar_entrada_con_uso0_modificado0(t_clock *self)
{
    int32_t posicionEntradaAReemplazar = -1;

    log_trace_with_mutex(logger, &mutex_logger, "Buscando entrada con (U=0, M=0)");

    for (int i = 0; i < config->marcos_por_proceso && posicionEntradaAReemplazar == -1; i++)
    {
        clock_print_entradas(self);

        t_entrada_segundonivel *entradaActualmenteApuntada = clock_obtener_entrada_actualmente_apuntada(self);
        if (entradaActualmenteApuntada->bit_uso == false && entradaActualmenteApuntada->bit_modificado == false)
        {
            posicionEntradaAReemplazar = self->puntero_aguja;
        }

        clock_apuntar_a_siguiente(self);
    }

    return posicionEntradaAReemplazar;
}

static int32_t clock_buscar_entrada_con_uso0_modificado1(t_clock *self)
{
    int32_t posicionEntradaAReemplazar = -1;

    log_trace_with_mutex(logger, &mutex_logger, "Buscando entrada con (U=0, M=1)");

    for (int i = 0; i < config->marcos_por_proceso && posicionEntradaAReemplazar == -1; i++)
    {
        clock_print_entradas(self);

        t_entrada_segundonivel *entradaActualmenteApuntada = clock_obtener_entrada_actualmente_apuntada(self);
        if (entradaActualmenteApuntada->bit_uso == false && entradaActualmenteApuntada->bit_modificado == true)
        {
            posicionEntradaAReemplazar = self->puntero_aguja;
        }
        else
        {
            entradaActualmenteApuntada->bit_uso = false;
        }

        clock_apuntar_a_siguiente(self);
    }

    return posicionEntradaAReemplazar;
}

static t_entrada_segundonivel *clock_obtener_entrada_actualmente_apuntada(t_clock *self)
{
    return clock_obtener_entrada_en_posicion(self, self->puntero_aguja);
}

static void clock_apuntar_a_siguiente(t_clock *self)
{
    self->puntero_aguja = (self->puntero_aguja + 1) % config->marcos_por_proceso;
}
