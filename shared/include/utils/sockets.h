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
 * @DESC: Crea un socket que escuche solicitudes de clientes en el puerto especificado
 * @RETURN: El socket servidor, que escucha por conexiones de clientes
 */
int iniciar_servidor(char *puerto, t_log *logger);

/**
 * @DESC: Se bloquea hasta que se conecte un cliente al socket
 * @RETURN: El socket de la conexion con el cliente
 */
int esperar_cliente(int socket_servidor);

/**
 * @DESC: Envia los datos del buffer a traves del socket
 * @RETURN: La cantidad de bytes enviados, o -1 en caso de error
 */
int enviar_por_socket(int socket, void *buffer_serializado, int bytes);

/**
 * @DESC: Envia un string a traves del socket
 * @RETURN: La cantidad de bytes enviados, o -1 en caso de error
 */
int enviar_string_por_socket(int socket, char *cadena);

/**
 * @DESC: Envia un uint32 a traves del socket
 * @RETURN: La cantidad de bytes enviados, o -1 en caso de error
 */
int enviar_uint32_por_socket(int socket, uint32_t numero);

/**
 * @DESC: Recibe del socket los bytes especificados y los almacena en el buffer
 * @RETURN: La cantidad de bytes verdaderamente recibidos
 */
int recibir_por_socket(int socket, void *buffer, int bytes);

/**
 * @DESC: Recibe del socket un uint32
 * @RETURN: La cantidad de bytes verdaderamente recibidos
 */
int recibir_uint32_por_socket(int socket, uint32_t *numero);

/**
 * @DESC: Libera la conexion de un socket
 */
void liberar_conexion(int socket);

#endif