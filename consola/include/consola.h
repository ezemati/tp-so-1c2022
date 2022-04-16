#ifndef CONSOLA_H
#define CONSOLA_H

#include "utils.h"
#include "tests.h"

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

void inicializar_consola(char **argv);
t_list *leer_instrucciones(FILE *archivo);
void agregar_instruccion(char *linea, t_list *instrucciones);
int crear_conexion_con_kernel(char *ip, char *puerto);
void enviar_instrucciones(int socket_kernel, void *instrucciones_serializadas, int bytes);
void terminar_consola();

#endif
