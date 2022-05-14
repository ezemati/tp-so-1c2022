#ifndef CPU_CONFIG_H
#define CPU_CONFIG_H

#include <stdint.h>
#include <stdlib.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/string.h>
#include <utils/logs.h>

typedef struct t_cpu_config
{
    uint32_t entradas_tbl;
    char *reemplazo_tbl;
    uint32_t retardo_noop;
    char *ip_memoria;
    uint32_t puerto_memoria;
    uint32_t puerto_escucha_dispatch;
    uint32_t puerto_escucha_interrupt;
} t_cpu_config;

t_cpu_config *cpu_config_new(char *config_path, t_log *logger);
void cpu_config_destroy(t_cpu_config *config);

#endif
