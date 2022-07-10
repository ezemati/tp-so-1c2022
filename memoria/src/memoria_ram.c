#include <memoria_ram.h>

static uint32_t memoria_ram_obtener_inicio_del_marco(uint32_t numero_marco);

static t_entrada_segundonivel *memoria_ram_obtener_entrada_segundonivel_en_direccion_fisica(t_memoria_ram *self, uint32_t direccion_fisica);

static void guardar_pagina_en_swap(t_memoria_ram *self, uint32_t pid, uint32_t numero_pagina, uint32_t numero_marco);
static void leer_pagina_de_swap(t_memoria_ram *self, uint32_t pid, uint32_t numero_pagina, uint32_t numero_marco);

t_memoria_ram *memoria_ram_new()
{
    t_memoria_ram *memoria = malloc(sizeof(t_memoria_ram));
    memoria->memoria_usuario = calloc(1, config->tamanio_memoria);

    uint32_t cantidad_marcos = memoria_ram_obtener_cantidad_marcos_totales();
    memoria->bitmap_marcos_libres = malloc(sizeof(bool) * cantidad_marcos);
    pthread_mutex_init(&(memoria->mutex_bitmap_marcos_libres), NULL);
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
    pthread_mutex_destroy(&(memoria->mutex_bitmap_marcos_libres));

    list_destroy_and_destroy_elements(memoria->tablas_primer_nivel, (void *)tabla_primernivel_destroy);

    list_destroy_and_destroy_elements(memoria->tablas_segundo_nivel, (void *)tabla_segundonivel_destroy);

    free(memoria);
}

uint32_t memoria_ram_agregar_proceso(t_memoria_ram *self, uint32_t pid, uint32_t tamanio_proceso)
{
    // Para simplificar, el numero de la tabla de 1N de cada proceso es igual a su PID
    uint32_t numero_tablaprimernivel_nuevoproceso = pid;
    t_tabla_primernivel *primernivel_nuevoproceso = tabla_primernivel_new(pid, tamanio_proceso, numero_tablaprimernivel_nuevoproceso, self->tablas_segundo_nivel);
    list_add(self->tablas_primer_nivel, primernivel_nuevoproceso);

    swap_crear_archivo(pid, tamanio_proceso);

    return numero_tablaprimernivel_nuevoproceso;
}

void memoria_ram_suspender_proceso(t_memoria_ram *self, uint32_t pid, uint32_t numero_tablaprimernivel)
{
    t_tabla_primernivel *tabla_primernivel = obtener_tablaprimernivel_por_numero(self->tablas_primer_nivel, numero_tablaprimernivel);

    for (int i = 0; i < config->entradas_por_tabla; i++)
    {
        uint32_t numero_tabla_segundonivel = tabla_primernivel->numeros_tablas_segundonivel[i];
        t_tabla_segundonivel *tabla_segundonivel = obtener_tablasegundonivel_por_numero(self->tablas_segundo_nivel, numero_tabla_segundonivel);
        t_list_iterator *iterator_tabla_segundonivel = list_iterator_create(tabla_segundonivel->entradas_segundonivel);
        while (list_iterator_has_next(iterator_tabla_segundonivel))
        {
            t_entrada_segundonivel *entrada_segundonivel = list_iterator_next(iterator_tabla_segundonivel);
            if (entrada_segundonivel_tiene_pagina_presente(entrada_segundonivel))
            {
                if (pagina_esta_modificada(entrada_segundonivel))
                {
                    guardar_pagina_en_swap(self, pid, entrada_segundonivel->numero_pagina, entrada_segundonivel->numero_marco);
                }

                memoria_ram_vaciar_marco(self, entrada_segundonivel->numero_marco);
                entrada_segundonivel_marcar_pagina_descargada(entrada_segundonivel);
            }
        }
        list_iterator_destroy(iterator_tabla_segundonivel);
    }

    clock_clear(tabla_primernivel->clock);
}

void memoria_ram_finalizar_proceso(t_memoria_ram *self, uint32_t numero_tablaprimernivel)
{
    t_tabla_primernivel *tablaprimernivel = obtener_tablaprimernivel_por_numero(self->tablas_primer_nivel, numero_tablaprimernivel);

    for (int i = 0; i < config->entradas_por_tabla; i++)
    {
        int numero_tablasegundonivel = tablaprimernivel->numeros_tablas_segundonivel[i];
        t_tabla_segundonivel *tablasegundonivel = obtener_tablasegundonivel_por_numero(self->tablas_segundo_nivel, numero_tablasegundonivel);

        t_list_iterator *iterator_entradassegundonivel = list_iterator_create(tablasegundonivel->entradas_segundonivel);
        while (list_iterator_has_next(iterator_entradassegundonivel))
        {
            t_entrada_segundonivel *entradasegundonivel = list_iterator_next(iterator_entradassegundonivel);
            if (entrada_segundonivel_tiene_pagina_presente(entradasegundonivel))
            {
                memoria_ram_vaciar_marco(self, entradasegundonivel->numero_marco);
                entrada_segundonivel_marcar_pagina_descargada(entradasegundonivel);
            }
        }
        list_iterator_destroy(iterator_entradassegundonivel);

        // Saca la tabla de 2N de la lista de tablas de 2N
        remover_tablasegundonivel_por_numero(self->tablas_segundo_nivel, numero_tablasegundonivel);

        // Libera recursos de la tabla de 2N y de sus entradas
        tabla_segundonivel_destroy(tablasegundonivel);
    }

    // Saca la tabla de 1N de la lista de tablas de 1N
    remover_tablaprimernivel_por_numero(self->tablas_primer_nivel, numero_tablaprimernivel);

    // Elimina el archivo SWAP del proceso
    swap_borrar_archivo(tablaprimernivel->pid);

    // Libera recursos de la tabla de 1N
    tabla_primernivel_destroy(tablaprimernivel);
}

void *memoria_ram_leer_dato(t_memoria_ram *self, t_memoria_leerdato_request *request)
{
    // TODO: ver si habria que validar que la pagina que se quiere leer este en memoria (en teoria ya se hace al pedir las direcciones para hacer la traduccion logica-fisica)
    uint32_t direccion_fisica = request->direccion_fisica;
    void *dato = malloc(request->cantidad_bytes);
    memcpy(dato, self->memoria_usuario + request->direccion_fisica, request->cantidad_bytes);

    t_entrada_segundonivel *pagina = memoria_ram_obtener_entrada_segundonivel_en_direccion_fisica(self, direccion_fisica);
    marcar_pagina_usada(pagina);

    return dato;
}

void memoria_ram_escribir_dato(t_memoria_ram *self, t_memoria_escribirdato_request *request)
{
    uint32_t direccion_fisica = request->direccion_fisica;
    memcpy(self->memoria_usuario + request->direccion_fisica, request->dato, request->cantidad_bytes);

    t_entrada_segundonivel *pagina = memoria_ram_obtener_entrada_segundonivel_en_direccion_fisica(self, direccion_fisica);
    marcar_pagina_usada(pagina);
    marcar_pagina_modificada(pagina);
}

uint32_t memoria_ram_obtener_numero_tabla_2_para_entrada_tabla_1(t_memoria_ram *self, t_memoria_numerotabla2paraentradatabla1_request *request)
{
    t_tabla_primernivel *tabla_primernivel = obtener_tablaprimernivel_por_numero(self->tablas_primer_nivel, request->numero_tablaprimernivel);
    uint32_t numero_tablasegundonivel = tabla_primernivel_obtener_numero_tabla_segundo_nivel_en_entrada(tabla_primernivel, self->tablas_segundo_nivel, request->entrada_tablaprimernivel);
    return numero_tablasegundonivel;
}

uint32_t memoria_ram_obtener_numero_marco_para_entrada_tabla_2(t_memoria_ram *self, t_memoria_marcoparaentradatabla2_request *request, int *numero_pagina_reemplazada)
{
    t_tabla_segundonivel *tabla_segundonivel = obtener_tablasegundonivel_por_numero(self->tablas_segundo_nivel, request->numero_tablasegundonivel);

    t_entrada_segundonivel *entrada_segundonivel = tabla_segundonivel_obtener_entrada_segundo_nivel(tabla_segundonivel, request->entrada_tablasegundonivel);
    if (!entrada_segundonivel_tiene_pagina_presente(entrada_segundonivel))
    {
        log_info_with_mutex(logger, &mutex_logger, "Entrada %d de tabla 2N %d (pagina %d) AUSENTE", request->entrada_tablasegundonivel, request->numero_tablasegundonivel, entrada_segundonivel->numero_pagina);
        t_tabla_primernivel *tablaprimernivel = obtener_tablaprimernivel_por_numero(self->tablas_primer_nivel, request->numero_tablaprimernivel);
        memoria_ram_cargar_pagina(self, entrada_segundonivel, tablaprimernivel->clock, numero_pagina_reemplazada);
    }

    return entrada_segundonivel->numero_marco;
}

void memoria_ram_cargar_pagina(t_memoria_ram *self, t_entrada_segundonivel *entrada_nueva, t_clock *clock, int *numero_pagina_reemplazada)
{
    *numero_pagina_reemplazada = -1;

    uint32_t pid = entrada_nueva->pid;

    if (!clock_esta_lleno(clock))
    {
        // Todavia no se asignaron todos los marcos al proceso, asi que puedo asignarle uno nuevo
        log_info_with_mutex(logger, &mutex_logger, "Asignando pagina %d a nuevo marco", entrada_nueva->numero_entrada);

        uint32_t numero_marco_libre = memoria_ram_obtener_numero_marco_libre(self);
        leer_pagina_de_swap(self, pid, entrada_nueva->numero_pagina, numero_marco_libre);
        entrada_segundonivel_marcar_pagina_cargada(entrada_nueva, numero_marco_libre);
        memoria_ram_marcar_marco_ocupado(self, numero_marco_libre);
        log_info_with_mutex(logger, &mutex_logger, "Pagina %d asignada a marco %d", entrada_nueva->numero_entrada, entrada_nueva->numero_marco);

        clock_agregar_entrada(clock, entrada_nueva);
        log_info_with_mutex(logger, &mutex_logger, "Nueva cantidad de marcos en memoria del proceso: %d/%d", clock_cantidad_entradas_llenas(clock), config->marcos_por_proceso);

        return;
    }

    log_info_with_mutex(logger, &mutex_logger, "El proceso %d ya tiene todos los marcos posibles asignados - Usando algoritmo de reemplazo %s", pid, config->algoritmo_reemplazo);

    uint32_t posicionEntradaAReemplazar = clock_obtener_posicion_pagina_a_reemplazar(clock);
    t_entrada_segundonivel *entrada_vieja = clock_obtener_entrada_en_posicion(clock, posicionEntradaAReemplazar);
    *numero_pagina_reemplazada = entrada_vieja->numero_pagina;
    log_info_with_mutex(logger, &mutex_logger, "Reemplazando marco %d", entrada_vieja->numero_marco);
    memoria_ram_reemplazar_pagina(self, pid, entrada_nueva, entrada_vieja);
    clock_reemplazar_entrada(clock, entrada_nueva, posicionEntradaAReemplazar);
}

void memoria_ram_reemplazar_pagina(t_memoria_ram *self, uint32_t pid, t_entrada_segundonivel *entrada_nueva, t_entrada_segundonivel *entrada_vieja)
{
    uint32_t numero_marco = entrada_vieja->numero_marco;

    if (pagina_esta_modificada(entrada_vieja))
    {
        guardar_pagina_en_swap(self, pid, entrada_vieja->numero_pagina, numero_marco);
    }

    entrada_segundonivel_marcar_pagina_descargada(entrada_vieja);
    memoria_ram_vaciar_marco(self, numero_marco);

    leer_pagina_de_swap(self, pid, entrada_nueva->numero_pagina, numero_marco);
    entrada_segundonivel_marcar_pagina_cargada(entrada_nueva, numero_marco);
    memoria_ram_marcar_marco_ocupado(self, numero_marco);
}

void memoria_ram_vaciar_marco(t_memoria_ram *self, uint32_t numero_marco)
{
    uint32_t bytes_comienzo_marco = memoria_ram_obtener_inicio_del_marco(numero_marco);
    memset(self->memoria_usuario + bytes_comienzo_marco, 0, config->tamanio_pagina);

    memoria_ram_marcar_marco_libre(self, numero_marco);
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
    pthread_mutex_lock(&(self->mutex_bitmap_marcos_libres));
    self->bitmap_marcos_libres[numero_marco] = false;
    pthread_mutex_unlock(&(self->mutex_bitmap_marcos_libres));
}

void memoria_ram_marcar_marco_libre(t_memoria_ram *self, uint32_t numero_marco)
{
    pthread_mutex_lock(&(self->mutex_bitmap_marcos_libres));
    self->bitmap_marcos_libres[numero_marco] = true;
    pthread_mutex_unlock(&(self->mutex_bitmap_marcos_libres));
}

uint32_t memoria_ram_obtener_cantidad_marcos_totales()
{
    return config->tamanio_memoria / config->tamanio_pagina;
}

bool memoria_ram_marco_esta_libre(t_memoria_ram *self, uint32_t numero_marco)
{
    pthread_mutex_lock(&(self->mutex_bitmap_marcos_libres));
    bool esta_libre = (self->bitmap_marcos_libres[numero_marco] == true);
    pthread_mutex_unlock(&(self->mutex_bitmap_marcos_libres));
    return esta_libre;
}

t_entrada_segundonivel *memoria_ram_obtener_pagina_en_marco(t_memoria_ram *self, uint32_t numero_marco)
{
    t_entrada_segundonivel *entrada_buscada = NULL;

    t_list_iterator *iterator_tablasprimernivel = list_iterator_create(self->tablas_primer_nivel);
    while (list_iterator_has_next(iterator_tablasprimernivel) && entrada_buscada == NULL)
    {
        t_tabla_primernivel *tablaprimernivel = list_iterator_next(iterator_tablasprimernivel);
        for (int i = 0; i < config->entradas_por_tabla && entrada_buscada == NULL; i++)
        {
            uint32_t numero_tablasegundonivel = tablaprimernivel->numeros_tablas_segundonivel[i];
            t_tabla_segundonivel *tablasegundonivel = obtener_tablasegundonivel_por_numero(self->tablas_segundo_nivel, numero_tablasegundonivel);
            t_list_iterator *iterator_entradassegundonivel = list_iterator_create(tablasegundonivel->entradas_segundonivel);
            while (list_iterator_has_next(iterator_entradassegundonivel) && entrada_buscada == NULL)
            {
                t_entrada_segundonivel *entradasegundonivel = list_iterator_next(iterator_entradassegundonivel);
                if (entrada_segundonivel_tiene_pagina_presente(entradasegundonivel) && entradasegundonivel->numero_marco == numero_marco)
                {
                    entrada_buscada = entradasegundonivel;
                }
            }
            list_iterator_destroy(iterator_entradassegundonivel);
        }
    }
    list_iterator_destroy(iterator_tablasprimernivel);

    return entrada_buscada;
}

static uint32_t memoria_ram_obtener_inicio_del_marco(uint32_t numero_marco)
{
    return numero_marco * config->tamanio_pagina;
}

static t_entrada_segundonivel *memoria_ram_obtener_entrada_segundonivel_en_direccion_fisica(t_memoria_ram *self, uint32_t direccion_fisica)
{
    uint32_t numero_marco = floor(direccion_fisica / config->tamanio_pagina);
    t_entrada_segundonivel *entrada = memoria_ram_obtener_pagina_en_marco(self, numero_marco);
    return entrada;
}

static void guardar_pagina_en_swap(t_memoria_ram *self, uint32_t pid, uint32_t numero_pagina, uint32_t numero_marco)
{
    uint32_t direccion_fisica_inicio_marco = memoria_ram_obtener_inicio_del_marco(numero_marco);
    void *contenido_pagina = calloc(1, config->tamanio_pagina);
    memcpy(contenido_pagina, self->memoria_usuario + direccion_fisica_inicio_marco, config->tamanio_pagina);

    swap_escribir_pagina(pid, numero_pagina, contenido_pagina);

    free(contenido_pagina);
}

static void leer_pagina_de_swap(t_memoria_ram *self, uint32_t pid, uint32_t numero_pagina, uint32_t numero_marco)
{
    uint32_t direccion_fisica_inicio_marco = memoria_ram_obtener_inicio_del_marco(numero_marco);
    void *contenido_pagina = swap_leer_pagina(pid, numero_pagina);
    memcpy(self->memoria_usuario + direccion_fisica_inicio_marco, contenido_pagina, config->tamanio_pagina);

    free(contenido_pagina);
}
