#ifndef CPU_REQUESTS_H
#define CPU_REQUESTS_H

#include <cpu_config.h>
#include <cpu_utils.h>
#include <cpu_variables_globales.h>

#include <cpu_info_ejecucion_actual.h>

#include <cpu/ejecutar_proceso.h>

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

void atender_ejecutar_proceso(int socket_cliente);

#endif
