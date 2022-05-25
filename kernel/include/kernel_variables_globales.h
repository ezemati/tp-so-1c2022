#ifndef KERNEL_VARIABLES_GLOBALES_H
#define KERNEL_VARIABLES_GLOBALES_H

struct t_kernel_config;

#include <commons/log.h>
#include <commons/collections/list.h>

extern struct t_kernel_config *config;
extern t_log *logger;
extern t_list *lista_procesos; // De tipo t_kernel_pcb
extern t_list *lista_ready;    // De tipo t_kernel_pcb (sus elementos pertenecen a lista_procesos)

#endif
