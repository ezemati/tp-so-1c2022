#ifndef KERNEL_VARIABLES_GLOBALES_H
#define KERNEL_VARIABLES_GLOBALES_H

struct t_kernel_config;

#include <semaphore.h>

#include <commons/log.h>
#include <commons/collections/list.h>

extern struct t_kernel_config *config;
extern t_log *logger;
extern t_list *lista_procesos;        // De tipo t_kernel_pcb
extern t_list *lista_ready;           // De tipo t_kernel_pcb (sus elementos pertenecen a lista_procesos)
extern t_list *lista_suspended_ready; // De tipo t_kernel_pcb (sus elementos pertenecen a lista_procesos)
extern t_list *lista_new;             // De tipo t_kernel_pcb (sus elementos pertenecen a lista_procesos)
extern t_list *lista_blocked;         // De tipo t_kernel_pcb (sus elementos pertenecen a lista_procesos), tiene tanto BLOCKED como SUSPENDED_BLOCKED

extern uint32_t proximo_pid;
extern int socket_conexion_cpu_dispatch;
extern bool hay_proceso_en_ejecucion;

extern pthread_mutex_t mutex_logger;
extern pthread_mutex_t mutex_proximo_pid;
extern pthread_mutex_t mutex_hay_proceso_en_ejecucion;

extern pthread_mutex_t mutex_lista_procesos;
extern pthread_mutex_t mutex_lista_ready;
extern pthread_mutex_t mutex_lista_suspended_ready;
extern pthread_mutex_t mutex_lista_new;
extern pthread_mutex_t mutex_lista_blocked;

extern sem_t sem_procesos_bloqueados;

#endif
