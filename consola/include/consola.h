#ifndef CONSOLA_H
#define CONSOLA_H

#include "utils.h"
#include "tests.h"
#include "consola_config.h"
#include <types/instruccion.h>

#include <netdb.h>
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
#include <utils/sockets.h>

void inicializar_consola(char **argv);
t_list *leer_instrucciones(FILE *archivo);
void agregar_instruccion(char *linea, t_list *instrucciones);
void terminar_consola();

#endif
