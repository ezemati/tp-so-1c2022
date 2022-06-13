#include <memoria_swap.h>

static FILE *obtener_archivo_swap_para_proceso(char *mode, uint32_t pid);
static char *obtener_path_completo_de_archivo(uint32_t pid);
static void mover_puntero_de_archivo_a_pagina(FILE *file, uint32_t numero_pagina);
static void bloquear_hilo_por_retardo_swap();

void swap_crear_archivo(uint32_t pid, uint32_t tamanio_proceso)
{
    sem_wait(&sem_swap_libre);

    FILE *file = obtener_archivo_swap_para_proceso("wb", pid);

    // Si el proceso es de 50 bytes y el tamanio de pagina es 40 bytes, el proceso
    // necesita 2 paginas, porque con 1 sola no le alcanza (fragmentacion interna?)
    uint32_t cantidad_paginas_del_proceso = ceil((float)tamanio_proceso / config->tamanio_pagina);
    uint32_t bytes_del_archivo_swap = cantidad_paginas_del_proceso * config->tamanio_pagina;

    void *bytes_vacios = malloc(bytes_del_archivo_swap);
    memset(bytes_vacios, 0, bytes_del_archivo_swap);
    fwrite(bytes_vacios, bytes_del_archivo_swap, 1, file);

    free(bytes_vacios);
    fclose(file);

    sem_post(&sem_swap_libre);
}

void *swap_leer_pagina(uint32_t pid, uint32_t numero_pagina)
{
    sem_wait(&sem_swap_libre);

    bloquear_hilo_por_retardo_swap();

    FILE *file = obtener_archivo_swap_para_proceso("rb", pid);

    void *contenido_pagina = malloc(config->tamanio_pagina);

    mover_puntero_de_archivo_a_pagina(file, numero_pagina);
    fread(contenido_pagina, config->tamanio_pagina, 1, file);

    fclose(file);

    log_trace_if_logger_not_null(logger, "(SWAP) PID %d: pagina %d leida", pid, numero_pagina);

    sem_post(&sem_swap_libre);

    return contenido_pagina;
}

void swap_escribir_pagina(uint32_t pid, uint32_t numero_pagina, void *contenido_pagina)
{
    sem_wait(&sem_swap_libre);

    bloquear_hilo_por_retardo_swap();

    FILE *file = obtener_archivo_swap_para_proceso("wb", pid);

    mover_puntero_de_archivo_a_pagina(file, numero_pagina);
    fwrite(contenido_pagina, config->tamanio_pagina, 1, file);

    fclose(file);

    log_trace_if_logger_not_null(logger, "(SWAP) PID %d: pagina %d escrita", pid, numero_pagina);

    sem_post(&sem_swap_libre);
}

void swap_borrar_archivo(uint32_t pid)
{
    sem_wait(&sem_swap_libre);

    char *full_path = obtener_path_completo_de_archivo(pid);
    remove(full_path);
    free(full_path);

    log_trace_if_logger_not_null(logger, "(SWAP) PID %d: archivo SWAP eliminado", pid);

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
    time_microseg retardo_swap_microsegundos = milisegundos_a_microsegundos(config->retardo_swap);
    usleep(retardo_swap_microsegundos);
}
