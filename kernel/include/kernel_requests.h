#ifndef KERNEL_REQUESTS_H
#define KERNEL_REQUESTS_H

#include <kernel_config.h>
#include <utils/sockets.h>
#include <types/identificador_operacion.h>

#include <kernel/request_proceso_consola.h>

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

void request_proceso_consola_new(int socket_cliente);

#endif
