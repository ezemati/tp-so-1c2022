#include <memoria_ram.h>

static t_tabla_primernivel *memoria_ram_obtener_tablaprimernivel(t_memoria_ram *self, uint32_t numero_tablaprimernivel);
static t_tabla_segundonivel *memoria_ram_obtener_tablasegundonivel(t_memoria_ram *self, uint32_t numero_tablasegundonivel);
static void memoria_ram_limpiar_marco(t_memoria_ram *self, uint32_t numero_marco);
static uint32_t memoria_ram_obtener_inicio_del_marco(uint32_t numero_marco);

static void memoria_ram_marcar_pagina_usada(t_memoria_ram *self, uint32_t numero_tablasegundonivel, uint32_t entrada_tablasegundonivel);
static void memoria_ram_marcar_pagina_modificada(t_memoria_ram *self, uint32_t numero_tablasegundonivel, uint32_t entrada_tablasegundonivel);

t_memoria_ram *memoria_ram_new()
{
    t_memoria_ram *memoria = malloc(sizeof(t_memoria_ram));
    memoria->memoria_usuario = malloc(config->tamanio_memoria);

    uint32_t cantidad_marcos = memoria_ram_obtener_cantidad_marcos_totales();
    memoria->bitmap_marcos_libres = malloc(sizeof(bool) * cantidad_marcos);
    for (int i = 0; i < cantidad_marcos; i++)
    {
        memoria->bitmap_marcos_libres[i] = true;
    }

    memoria->tablas_primer_nivel = list_create();
    memoria->tablas_segundo_nivel = list_create();
    return memoria;
}

void memoria_ram_destroy(t_memoria_ram *memoria)
{
    free(memoria->memoria_usuario);
    free(memoria->bitmap_marcos_libres);

    list_destroy_and_destroy_elements(memoria->tablas_primer_nivel, (void *)tabla_primernivel_destroy);
    free(memoria->tablas_primer_nivel);

    list_destroy_and_destroy_elements(memoria->tablas_segundo_nivel, (void *)tabla_segundonivel_destroy);
    free(memoria->tablas_segundo_nivel);

    free(memoria);
}

uint32_t memoria_ram_agregar_proceso(t_memoria_ram *self, uint32_t pid, uint32_t tamanio_proceso)
{
    uint32_t numero_tablaprimernivel_nuevoproceso = list_size(self->tablas_primer_nivel);
    t_tabla_primernivel *primernivel_nuevoproceso = tabla_primernivel_new(pid, tamanio_proceso, numero_tablaprimernivel_nuevoproceso, self->tablas_segundo_nivel);
    list_add(self->tablas_primer_nivel, primernivel_nuevoproceso);
    return numero_tablaprimernivel_nuevoproceso;
}

void memoria_ram_finalizar_proceso(t_memoria_ram *self, uint32_t numero_tablaprimernivel)
{
    t_tabla_primernivel *tablaPrimerNivel = memoria_ram_obtener_tablaprimernivel(self, numero_tablaprimernivel);
    for (int i = 0; i < config->entradas_por_tabla; i++)
    {
        int indiceTablaSegundoNivel = tablaPrimerNivel->indices_tablas_segundonivel[i];
        t_tabla_segundonivel *tablaSegundoNivel = memoria_ram_obtener_tablasegundonivel(self, indiceTablaSegundoNivel);
        t_list_iterator *iteratorEntradasSegundoNivel = list_iterator_create(tablaSegundoNivel->entradas_segundonivel);
        while (list_iterator_has_next(iteratorEntradasSegundoNivel))
        {
            t_entrada_segundonivel *entradaSegundoNivel = list_iterator_next(iteratorEntradasSegundoNivel);
            if (entrada_segundonivel_tiene_pagina_presente(entradaSegundoNivel))
            {
                memoria_ram_limpiar_marco(self, entradaSegundoNivel->numero_marco);
            }
            entrada_segundonivel_marcar_pagina_descargada(entradaSegundoNivel);
        }
        list_iterator_destroy(iteratorEntradasSegundoNivel);
    }

    // TODO: falta liberar lo del SWAP
}

void *memoria_ram_leer_dato(t_memoria_ram *self, t_memoria_leerdato_request *request)
{
    // TODO: ver si habria que validar que la pagina que se quiere leer este en memoria (en teoria ya se hace al pedir las direcciones para hacer la traduccion logica-fisica)
    void *dato = malloc(request->cantidad_bytes);
    memcpy(dato, self->memoria_usuario + request->direccion_fisica, request->cantidad_bytes);

    memoria_ram_marcar_pagina_usada(self, request->numero_tablasegundonivel, request->entrada_tablasegundonivel);
    return dato;
}

void memoria_ram_escribir_dato(t_memoria_ram *self, t_memoria_escribirdato_request *request)
{
    memcpy(self->memoria_usuario + request->direccion_fisica, request->dato, request->cantidad_bytes);

    memoria_ram_marcar_pagina_usada(self, request->numero_tablasegundonivel, request->entrada_tablasegundonivel);
    memoria_ram_marcar_pagina_modificada(self, request->numero_tablasegundonivel, request->entrada_tablasegundonivel);
}

uint32_t memoria_ram_obtener_numero_tabla_2_para_entrada_tabla_1(t_memoria_ram *self, t_memoria_numerotabla2paraentradatabla1_request *request)
{
    t_tabla_primernivel *tablaPrimerNivel = memoria_ram_obtener_tablaprimernivel(self, request->numero_tablaprimernivel);

    uint32_t numero_tablasegundonivel = tabla_primernivel_obtener_numero_tabla_segundo_nivel_en_entrada(tablaPrimerNivel, self->tablas_segundo_nivel, request->entrada_tablaprimernivel);

    return numero_tablasegundonivel;
}

uint32_t memoria_ram_obtener_numero_marco_para_entrada_tabla_2(t_memoria_ram *self, t_memoria_marcoparaentradatabla2_request *request)
{
    //TODO: ver si la pagina esta presente, si no cargarla (usar algoritmos de reemplazo)
    t_tabla_segundonivel *tablaSegundoNivel = memoria_ram_obtener_tablasegundonivel(self, request->numero_tablasegundonivel);

    t_entrada_segundonivel *entrada_segundonivel = tabla_segundonivel_obtener_entrada_segundo_nivel(tablaSegundoNivel, request->entrada_tablasegundonivel);
    if (!entrada_segundonivel_tiene_pagina_presente(entrada_segundonivel))
    {
        t_tabla_primernivel *tablaPrimerNivel = memoria_ram_obtener_tablaprimernivel(self, request->numero_tablaprimernivel);
        memoria_ram_cargar_pagina(self, entrada_segundonivel, tablaPrimerNivel->clock);
    }

    return entrada_segundonivel->numero_marco;
}

void memoria_ram_cargar_pagina(t_memoria_ram *self, t_entrada_segundonivel *entradaNueva, t_clock *clock)
{
    if (!clock_esta_lleno(clock))
    {
        // Todavia no se asignaron todos los marcos al proceso, asi que puedo asignarle uno nuevo
        uint32_t numero_marco_libre = memoria_ram_obtener_numero_marco_libre(self);
        entrada_segundonivel_marcar_pagina_cargada(entradaNueva, numero_marco_libre);
        memoria_ram_marcar_marco_ocupado(self, numero_marco_libre);
        
        clock_agregar_entrada(clock, entradaNueva);
        return;
    }

    uint32_t posicionEntradaAReemplazar = clock_obtener_posicion_pagina_a_reemplazar(clock);
    t_entrada_segundonivel *entradaVieja = clock_obtener_entrada_en_posicion(clock, posicionEntradaAReemplazar);
    memoria_ram_reemplazar_pagina(self, entradaNueva, entradaVieja);
    clock_reemplazar_entrada(clock, entradaNueva, posicionEntradaAReemplazar);
}

void memoria_ram_reemplazar_pagina(t_memoria_ram *self, t_entrada_segundonivel *entradaNueva, t_entrada_segundonivel *entradaVieja)
{
    if (entradaVieja->bit_modificado == true)
    {
        // TODO: guardar las modificaciones a la pagina que va a ser reemplazada
    }

    uint32_t numero_marco = entradaVieja->numero_marco;
    entrada_segundonivel_marcar_pagina_descargada(entradaVieja);
    memoria_ram_vaciar_marco(self, numero_marco);
    entrada_segundonivel_marcar_pagina_cargada(entradaNueva, numero_marco);
    memoria_ram_marcar_marco_ocupado(self, numero_marco);
}

void memoria_ram_vaciar_marco(t_memoria_ram *self, uint32_t numero_marco)
{
    uint32_t bytes_comienzo_marco = numero_marco * config->tamanio_pagina;
    memset(self->memoria_usuario + bytes_comienzo_marco, 0, config->tamanio_memoria);
}

int32_t memoria_ram_obtener_numero_marco_libre(t_memoria_ram *self)
{
    uint32_t cantidad_marcos = memoria_ram_obtener_cantidad_marcos_totales();

    for (int i = 0; i < cantidad_marcos; i++)
    {
        if (memoria_ram_marco_esta_libre(self, i))
        {
            return i;
        }
    }

    return -1;
}

void memoria_ram_marcar_marco_ocupado(t_memoria_ram *self, uint32_t numero_marco)
{
    self->bitmap_marcos_libres[numero_marco] = false;
}

uint32_t memoria_ram_obtener_cantidad_marcos_totales()
{
    return config->tamanio_memoria / config->tamanio_pagina;
}

bool memoria_ram_marco_esta_libre(t_memoria_ram *self, uint32_t numero_marco)
{
    return self->bitmap_marcos_libres[numero_marco] == true;
}

t_entrada_segundonivel *memoria_ram_obtener_pagina_en_marco(t_memoria_ram *self, uint32_t numero_marco)
{
    t_entrada_segundonivel *entradaBuscada = NULL;

    t_list_iterator *iteratorTablasPrimerNivel = list_iterator_create(self->tablas_primer_nivel);
    while (list_iterator_has_next(iteratorTablasPrimerNivel) && entradaBuscada == NULL)
    {
        t_tabla_primernivel *tablaPrimerNivel = list_iterator_next(iteratorTablasPrimerNivel);
        for (int i = 0; i < config->entradas_por_tabla && entradaBuscada == NULL; i++)
        {
            uint32_t indiceTablaSegundoNivel = tablaPrimerNivel->indices_tablas_segundonivel[i];
            t_tabla_segundonivel *tablaSegundoNivel = memoria_ram_obtener_tablasegundonivel(self, indiceTablaSegundoNivel);
            t_list_iterator *iteratorEntradasSegundoNivel = list_iterator_create(tablaSegundoNivel->entradas_segundonivel);
            while (list_iterator_has_next(iteratorEntradasSegundoNivel) && entradaBuscada == NULL)
            {
                t_entrada_segundonivel *entradaSegundoNivel = list_iterator_next(iteratorEntradasSegundoNivel);
                if (entrada_segundonivel_tiene_pagina_presente(entradaSegundoNivel) && entradaSegundoNivel->numero_marco == numero_marco)
                {
                    entradaBuscada = entradaSegundoNivel;
                }
            }
            list_iterator_destroy(iteratorEntradasSegundoNivel);
        }
    }
    list_iterator_destroy(iteratorTablasPrimerNivel);

    return entradaBuscada;
}

static t_tabla_primernivel *memoria_ram_obtener_tablaprimernivel(t_memoria_ram *self, uint32_t numero_tablaprimernivel)
{
    return list_get(self->tablas_primer_nivel, numero_tablaprimernivel);
}

static t_tabla_segundonivel *memoria_ram_obtener_tablasegundonivel(t_memoria_ram *self, uint32_t numero_tablasegundonivel)
{
    return list_get(self->tablas_segundo_nivel, numero_tablasegundonivel);
}

static void memoria_ram_limpiar_marco(t_memoria_ram *self, uint32_t numero_marco)
{
    uint32_t inicioMarco = memoria_ram_obtener_inicio_del_marco(numero_marco);
    memset(self->memoria_usuario + inicioMarco, 0, config->tamanio_pagina);
}

static uint32_t memoria_ram_obtener_inicio_del_marco(uint32_t numero_marco)
{
    return numero_marco * config->tamanio_pagina;
}

static void memoria_ram_marcar_pagina_usada(t_memoria_ram *self, uint32_t numero_tablasegundonivel, uint32_t entrada_tablasegundonivel)
{
    t_tabla_segundonivel *tabla = memoria_ram_obtener_tablasegundonivel(self, numero_tablasegundonivel);
    t_entrada_segundonivel *entrada = tabla_segundonivel_obtener_entrada_segundo_nivel(tabla, entrada_tablasegundonivel);

    entrada->bit_uso = true;
}

static void memoria_ram_marcar_pagina_modificada(t_memoria_ram *self, uint32_t numero_tablasegundonivel, uint32_t entrada_tablasegundonivel)
{
    t_tabla_segundonivel *tabla = memoria_ram_obtener_tablasegundonivel(self, numero_tablasegundonivel);
    t_entrada_segundonivel *entrada = tabla_segundonivel_obtener_entrada_segundo_nivel(tabla, entrada_tablasegundonivel);

    entrada->bit_modificado = true;
}
