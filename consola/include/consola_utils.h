#ifndef CONSOLA_UTILS_H
#define CONSOLA_UTILS_H

#include <consola_config.h>
#include <utils/sockets.h>
#include <utils/string.h>

#include <types/instruccion.h>
#include <types/programa.h>

#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <commons/collections/list.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/txt.h>

void inicializar_consola(int argc, char **argv);
void terminar_consola();

extern t_log *logger;
extern t_consola_config *config;
extern t_programa *programa;

extern pthread_mutex_t mutex_logger;

#endif
