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

extern int socket_servidor;
extern int socket_memoria;
extern int socket_cpu_dispatch;
extern int socket_cpu_interrupt;

extern pthread_mutex_t mutex_lista_procesos;
extern pthread_mutex_t mutex_lista_ready;
extern pthread_mutex_t mutex_lista_suspended_ready;
extern pthread_mutex_t mutex_lista_new;

#endif
