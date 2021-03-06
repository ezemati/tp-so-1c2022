#ifndef UTILS_SOCKETS_H
#define UTILS_SOCKETS_H

#include <serialization/serialization_utils.h>
#include <types/identificador_operacion.h>
#include <types/time_types.h>
#include <utils/converter.h>
#include <utils/logs.h>
#include <utils/string.h>

#include <netdb.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <commons/log.h>

/**
 * @brief Crea una conexion con un socket en la IP y PUERTO especificado
 * @return El fd del socket, o -1 en caso de error
 */
int crear_conexion(char *ip, uint32_t puerto, t_log *logger, pthread_mutex_t *mutex_logger);

/**
 * @brief Crea un socket que escuche solicitudes de clientes en el puerto especificado
 * @return El socket servidor, que escucha por conexiones de clientes
 */
int iniciar_servidor(uint32_t puerto, t_log *logger, pthread_mutex_t *mutex_logger);

/**
 * @brief Se bloquea hasta que se conecte un cliente al socket
 * @return El socket de la conexion con el cliente
 */
int esperar_cliente(int socket_servidor);

/**
 * @brief Envia el buffer serializado por socket, agregandole como cabecera un uint32 con el tamanio en bytes del buffer (para despues saber cuantos bytes recibir del socket)
 */
int enviar_buffer_serializado_con_bytes_por_socket(int socket, void *buffer_serializado, int bytes);

/**
 * @brief Envia los datos del buffer a traves del socket
 * @return La cantidad de bytes enviados, o -1 en caso de error
 */
int enviar_buffer_por_socket(int socket, void *buffer_serializado, int bytes);

/**
 * @brief Envia un buffer para una operacion a traves del socket, agregandole como cabecera un uint32 con el codigo de la operacion, y otro uint32 con el tamanio en bytes del buffer
 */
int enviar_buffer_serializado_con_instruccion_y_bytes_por_socket(int socket, identificador_operacion operacion, void *buffer_serializado, int bytes);

/**
 * @brief Envia un string a traves del socket
 * @return La cantidad de bytes enviados, o -1 en caso de error
 */
int enviar_string_por_socket(int socket, char *cadena);

/**
 * @brief Envia un string con su longitud en uint32 a traves del socket
 * @return La cantidad de bytes de la cadena enviados, o -1 en caso de error
 */
int enviar_string_con_longitud_por_socket(int socket, char *cadena);

/**
 * @brief Envia un uint32 a traves del socket
 * @return La cantidad de bytes enviados, o -1 en caso de error
 */
int enviar_uint32_por_socket(int socket, uint32_t numero);

/**
 * @brief Recibe del socket los bytes especificados y los almacena en el buffer
 * @return La cantidad de bytes verdaderamente recibidos
 */
int recibir_buffer_por_socket(int socket, void *buffer, int bytes);

/**
 * @brief Recibe del socket un buffer cuya cabecera indica el tamanio en bytes de dicho buffer
 */
int recibir_buffer_con_bytes_por_socket(int socket, void **buffer);

/**
 * @brief Recibe del socket un uint32
 * @return La cantidad de bytes verdaderamente recibidos
 */
int recibir_uint32_por_socket(int socket, uint32_t *numero);

/**
 * @brief Recibe del socket un string con una longitud especificada en uint32
 * @return La cantidad de bytes del string verdaderamente recibidos
 */
int recibir_string_con_longitud_por_socket(int socket, char **buffer);

/**
 * @brief Libera la conexion de un socket
 */
void liberar_conexion(int socket);

#endif
