#include <memoria_swap.h>

static FILE *obtener_archivo_swap_para_proceso(char *mode, uint32_t pid);
static char *obtener_path_completo_de_archivo(uint32_t pid);
static void mover_puntero_de_archivo_a_pagina(FILE *file, uint32_t numero_pagina);
static void bloquear_hilo_por_retardo_swap();
static uint32_t calcular_bytes_de_pagina(uint32_t numero_pagina, uint32_t tamanio_proceso);

void swap_crear_archivo(uint32_t pid, uint32_t tamanio_proceso)
{
    sem_wait(&sem_swap_libre);

    // El modo "w" trunca el archivo y lo vacia
    FILE *file = obtener_archivo_swap_para_proceso("wb", pid);

    void *bytes_vacios = calloc(1, tamanio_proceso);
    fwrite(bytes_vacios, tamanio_proceso, 1, file);

    free(bytes_vacios);
    fclose(file);

    sem_post(&sem_swap_libre);
}

void *swap_leer_pagina(uint32_t pid, uint32_t numero_pagina)
{
    sem_wait(&sem_swap_libre);

    bloquear_hilo_por_retardo_swap();

    FILE *file = obtener_archivo_swap_para_proceso("rb", pid);

    void *contenido_pagina = calloc(1, config->tamanio_pagina);

    uint32_t tamanio_proceso = file_size(file);
    uint32_t bytes_a_leer = calcular_bytes_de_pagina(numero_pagina, tamanio_proceso);

    log_trace_with_mutex(logger, &mutex_logger, "(SWAP) PID %d: leyendo %d bytes de la pagina %d", pid, bytes_a_leer, numero_pagina);
    mover_puntero_de_archivo_a_pagina(file, numero_pagina);
    fread(contenido_pagina, bytes_a_leer, 1, file);

    fclose(file);

    log_info_with_mutex(logger, &mutex_logger, "(SWAP) PID %d: pagina %d leida", pid, numero_pagina);

    sem_post(&sem_swap_libre);

    return contenido_pagina;
}

void swap_escribir_pagina(uint32_t pid, uint32_t numero_pagina, void *contenido_pagina)
{
    sem_wait(&sem_swap_libre);

    bloquear_hilo_por_retardo_swap();

    FILE *file = obtener_archivo_swap_para_proceso("r+b", pid);

    uint32_t tamanio_proceso = file_size(file);
    uint32_t bytes_a_escribir = calcular_bytes_de_pagina(numero_pagina, tamanio_proceso);

    log_trace_with_mutex(logger, &mutex_logger, "(SWAP) PID %d: escribiendo %d bytes en la pagina %d", pid, bytes_a_escribir, numero_pagina);
    mover_puntero_de_archivo_a_pagina(file, numero_pagina);
    fwrite(contenido_pagina, bytes_a_escribir, 1, file);

    fclose(file);

    log_info_with_mutex(logger, &mutex_logger, "(SWAP) PID %d: pagina %d escrita", pid, numero_pagina);

    sem_post(&sem_swap_libre);
}

void swap_borrar_archivo(uint32_t pid)
{
    sem_wait(&sem_swap_libre);

    char *full_path = obtener_path_completo_de_archivo(pid);
    remove(full_path);
    free(full_path);

    log_trace_with_mutex(logger, &mutex_logger, "(SWAP) PID %d: archivo SWAP eliminado", pid);

    sem_post(&sem_swap_libre);
}

static FILE *obtener_archivo_swap_para_proceso(char *mode, uint32_t pid)
{
    return fopen_with_format(mode, "%s/%d.swap", config->path_swap, pid);
}

static char *obtener_path_completo_de_archivo(uint32_t pid)
{
    char *full_path = malloc(sizeof(char) * 500);
    sprintf(full_path, "%s/%d.swap", config->path_swap, pid);
    return full_path;
}

static void mover_puntero_de_archivo_a_pagina(FILE *file, uint32_t numero_pagina)
{
    // Si quiero leer la pagina 2, me tengo que saltear 2 paginas (la 0 y la 1)
    long bytes_a_saltear = numero_pagina * config->tamanio_pagina;
    fseek(file, bytes_a_saltear, SEEK_SET);
}

static void bloquear_hilo_por_retardo_swap()
{
    log_trace_with_mutex(logger, &mutex_logger, "(SWAP) Empezando RETARDO_SWAP (%lldms)", config->retardo_swap);
    time_microseg retardo_swap_microsegundos = milisegundos_a_microsegundos(config->retardo_swap);
    usleep(retardo_swap_microsegundos);
    log_trace_with_mutex(logger, &mutex_logger, "(SWAP) RETARDO_SWAP finalizado");
}

static uint32_t calcular_bytes_de_pagina(uint32_t numero_pagina, uint32_t tamanio_proceso)
{
    // Ejemplo: si un proceso tiene un tamanio de 13 bytes y paginas de 5 bytes, va a tener tres
    // paginas, una de 0-4 (5 bytes), otra de 5-9 (5 bytes) y otra incompleta de 10-12 (3 bytes).
    // Si queremos leer la 3ra pagina (numero_pagina=2), solamente tenemos que leer 3 bytes, y no 5.
    // El inicio de la pagina va a ser el byte 10, asi que podemos hacer `13 - 10 = 3`
    uint32_t byte_inicio_pagina = numero_pagina * config->tamanio_pagina;
    uint32_t bytes_restantes_en_pagina = fmin(tamanio_proceso - byte_inicio_pagina, config->tamanio_pagina);
    return bytes_restantes_en_pagina;
}
