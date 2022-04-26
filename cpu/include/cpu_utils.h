#ifndef CPU_UTILS_H
#define CPU_UTILS_H

#include <cpu_config.h>
#include <utils/sockets.h>

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
int crear_conexion(char *ip, char *puerto, t_log *logger);
int enviar_string_por_socket(int socket, char *cadena);
void liberar_conexion(int socket);

extern t_log *logger;
extern t_cpu_config *config;

#endif
