#ifndef CPU_VARIABLES_GLOBALES_H
#define CPU_VARIABLES_GLOBALES_H

struct t_cpu_config;
struct t_cpu_info_ejecucion_actual;

#include <stdbool.h>
#include <commons/log.h>
#include <commons/collections/list.h>

extern struct t_cpu_config *config;
extern t_log *logger;
extern struct t_cpu_info_ejecucion_actual *info_ejecucion_actual;

extern bool hay_interrupcion;

extern int socket_memoria;

#endif
