#ifndef UTILS_SOCKETS_H
#define UTILS_SOCKETS_H

#include <utils/logs.h>

#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <commons/log.h>

/**
 * @DESC: Crea una conexion con un socket en la IP y PUERTO especificado
 * @RETURN: El fd del socket, o -1 en caso de error
 */
int crear_conexion(char *ip, char *puerto, t_log *logger);

/**
 * @DESC: Envia los datos del buffer a traves del socket
 * @RETURN: La cantidad de bytes enviados, o -1 en caso de error
 */
int enviar_por_socket(int socket, void *buffer_serializado, int bytes);

/**
 * @DESC: Libera la conexion de un socket
 */
void liberar_conexion(int socket);

#endif