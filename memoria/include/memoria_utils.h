#ifndef MEMORIA_UTILS_H
#define MEMORIA_UTILS_H

#include <memoria_config.h>
#include <memoria_ram.h>
#include <memoria_requests.h>

#include <utils/sockets.h>
#include <types/identificador_operacion.h>

#include <netdb.h>
#include <stdbool.h>
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

void inicializar_memoria(int argc, char **argv);
void terminar_memoria();
void procesar_request(int socket_cliente);
void *procesar_cliente(uint32_t *args);

#endif
