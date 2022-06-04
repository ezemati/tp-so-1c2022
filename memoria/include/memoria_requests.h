#ifndef MEMORIA_REQUESTS_H
#define MEMORIA_REQUESTS_H

#include <memoria_config.h>
#include <memoria_ram.h>
#include <utils/sockets.h>
#include <types/identificador_operacion.h>

#include <memoria/escribir_dato.h>
#include <memoria/finalizar_proceso.h>
#include <memoria/handshake_configuraciones.h>
#include <memoria/inicializar_proceso.h>
#include <memoria/leer_dato.h>
#include <memoria/marco_para_entrada_tabla_2.h>
#include <memoria/numero_tabla_2_para_entrada_tabla_1.h>
#include <memoria/suspender_proceso.h>

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

void inicializar_proceso(int socket_cliente);
void suspender_proceso(int socket_cliente);
void finalizar_proceso(int socket_cliente);
void handshake_info_traduccion(int socket_cliente);
void leer_dato(int socket_cliente);
void escribir_dato(int socket_cliente);
void obtener_numero_tabla_2_para_entrada_tabla_1(int socket_cliente);
void obtener_marco_para_entrada_tabla_2(int socket_cliente);

#endif
