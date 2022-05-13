#ifndef KERNEL_CONFIG_H
#define KERNEL_CONFIG_H

#include <stdint.h>
#include <stdlib.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/string.h>
#include <utils/logs.h>

typedef struct t_kernel_config
{
    char *ip_memoria;
    uint32_t puerto_memoria;
    char *ip_cpu;
    uint32_t puerto_cpu_dispatch;
    uint32_t puerto_cpu_interrupt;
    uint32_t puerto_escucha;
    char *algoritmo_planificacion;
    uint32_t estimacion_inicial;
    uint32_t alfa;
    uint32_t grado_multiprogramacion;
    uint32_t tiempo_maximo_bloqueado;
} t_kernel_config;

t_kernel_config *kernel_config_new(char *config_path, t_log *logger);

void kernel_config_destroy(t_kernel_config *config);

#endif
