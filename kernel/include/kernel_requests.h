#ifndef KERNEL_REQUESTS_H
#define KERNEL_REQUESTS_H

#include <kernel_config.h>
#include <kernel_utils.h>
#include <kernel_pcb.h>
#include <kernel_variables_globales.h>

#include <kernel/actualizar_pcb.h>

#include <types/programa.h>

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
#include <commons/log.h>
#include <commons/string.h>
#include <commons/txt.h>

void atender_crear_proceso(int socket_cliente);
void atender_bloquear_proceso(int socket_cliente);
void atender_finalizar_proceso(int socket_cliente);

#endif
