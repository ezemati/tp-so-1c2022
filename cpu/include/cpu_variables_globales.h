#ifndef CPU_VARIABLES_GLOBALES_H
#define CPU_VARIABLES_GLOBALES_H

struct t_cpu_config;

#include <stdbool.h>
#include <commons/log.h>
#include <commons/collections/list.h>

extern struct t_cpu_config *config;
extern t_log *logger;
extern bool hay_interrupcion;

#endif
