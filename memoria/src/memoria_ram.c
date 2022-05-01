#include <memoria_ram.h>

static t_tabla_primernivel *memoria_ram_obtener_tablaprimernivel(t_memoria_ram *self, uint32_t numero_tablaprimernivel);
static t_tabla_segundonivel *memoria_ram_obtener_tablasegundonivel(t_memoria_ram *self, uint32_t numero_tablasegundonivel);
static void memoria_ram_limpiar_marco(t_memoria_ram *self, uint32_t numero_marco);
static uint32_t memoria_ram_obtener_inicio_del_marco(uint32_t numero_marco);

t_memoria_ram *memoria_ram_new()
{
    t_memoria_ram *memoria = malloc(sizeof(t_memoria_ram));
    memoria->memoria_usuario = malloc(config->tamanio_memoria);
    memoria->tablas_primer_nivel = list_create();
    memoria->tablas_segundo_nivel = list_create();
    return memoria;
}

void memoria_ram_destroy(t_memoria_ram *memoria)
{
    free(memoria->memoria_usuario);

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
            entradaSegundoNivel->bit_presencia = 0;
            entradaSegundoNivel->numero_marco = -1;
        }
        list_iterator_destroy(iteratorEntradasSegundoNivel);
    }

    // TODO: falta liberar lo del SWAP
}

void *memoria_ram_leer_dato(t_memoria_ram *self, t_memoria_leerdato_request *request){
    // TODO: ver si habria que validar que la pagina que se quiere leer este en memoria (en teoria ya se hace al pedir las direcciones para hacer la traduccion logica-fisica)
    void *dato = malloc(request->cantidad_bytes);
    memcpy(dato, self->memoria_usuario + request->direccion_fisica, request->cantidad_bytes);
    return dato;
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
