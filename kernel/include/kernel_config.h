#ifndef KERNEL_CONFIG_H
#define KERNEL_CONFIG_H

#include <types/time_types.h>
#include <utils/logs.h>

#include <stdint.h>
#include <stdlib.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/string.h>

typedef struct t_kernel_config
{
    char *ip_memoria;
    uint32_t puerto_memoria;
    char *ip_cpu;
    uint32_t puerto_cpu_dispatch;
    uint32_t puerto_cpu_interrupt;
    uint32_t puerto_escucha;
    char *algoritmo_planificacion;
    time_miliseg estimacion_inicial;
    double alfa;
    uint32_t grado_multiprogramacion;
    time_miliseg tiempo_maximo_bloqueado;
} t_kernel_config;

t_kernel_config *kernel_config_new(char *config_path);

void kernel_config_destroy(t_kernel_config *config);

#endif
