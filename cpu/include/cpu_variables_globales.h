#ifndef CPU_VARIABLES_GLOBALES_H
#define CPU_VARIABLES_GLOBALES_H

struct t_cpu_config;
struct t_cpu_info_ejecucion_actual;
struct t_cpu_tlb;

#include <stdbool.h>
#include <commons/log.h>
#include <commons/collections/list.h>

extern struct t_cpu_config *config;
extern t_log *logger;
extern struct t_cpu_info_ejecucion_actual *info_ejecucion_actual;
extern struct t_cpu_tlb *tlb;

extern bool hay_interrupcion;
extern bool hay_proceso_en_ejecucion;
extern int socket_conexion_kernel_dispatch;
extern int socket_conexion_kernel_interrupt;

extern pthread_mutex_t mutex_hay_interrupcion;
extern pthread_mutex_t mutex_hay_proceso_en_ejecucion;

#endif
