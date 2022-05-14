#ifndef CPU_UTILS_H
#define CPU_UTILS_H

#include <cpu_config.h>
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
#include <sys/socket.h>


void inicializar_cpu(char **argv);
void terminar_cpu();

extern t_log *logger;
extern t_cpu_config *config;

#endif
