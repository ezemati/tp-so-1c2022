#ifndef CONSOLA_UTILS_H
#define CONSOLA_UTILS_H

#include <consola_config.h>
#include <utils/sockets.h>
#include <utils/string.h>

#include <types/instruccion.h>
#include <types/programa.h>

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

void inicializar_consola(int argc, char **argv);
t_list *leer_instrucciones(FILE *archivo);
void agregar_instruccion(char *linea, t_list *instrucciones);
void instrucciones_destroy(t_list *instrucciones);
void instruccion_buffer_destroy(void *buffer_instruccion);
void terminar_consola();

extern t_log *logger;
extern t_consola_config *config;
extern t_programa *programa;

#endif
