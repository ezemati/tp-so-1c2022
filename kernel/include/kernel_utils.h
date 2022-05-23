#ifndef KERNEL_UTILS_H
#define KERNEL_UTILS_H

#include <kernel_config.h>
#include <kernel_pcb.h>
#include <kernel_requests.h>
#include <kernel_variables_globales.h>

#include <utils/list.h>
#include <utils/sockets.h>
#include <types/identificador_operacion.h>

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <commons/collections/list.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/txt.h>

// Funciones del Kernel como servidor
void inicializar_kernel(char **argv);
void terminar_kernel();
void procesar_request(int socket_cliente);
void *procesar_cliente(uint32_t *args);

uint32_t obtener_proximo_pid();
void print_instrucciones(t_kernel_pcb *pcb);
void print_instrucciones_de_todos_los_procesos(t_list *pcbs);

#endif
