#ifndef CPU_UTILS_H
#define CPU_UTILS_H

#include <cpu_config.h>
#include <cpu_requests.h>
#include <cpu_variables_globales.h>

#include <cpu_ejecucion_instrucciones.h>

#include <utils/sockets.h>
#include <types/identificador_operacion.h>

#include <memoria/handshake_configuraciones.h>
#include <memoria/numero_tabla_2_para_entrada_tabla_1.h>
#include <memoria/marco_para_entrada_tabla_2.h>

#include <math.h>
#include <netdb.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>
#include <commons/collections/list.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/string.h>

void inicializar_cpu(char **argv);
void terminar_cpu();
void procesar_request(int socket_cliente);
void *procesar_cliente(void *args);

void realizar_handshake_con_memoria(t_cpu_config *config);
void realizar_ejecucion();

uint32_t traducir_direccion_logica_a_fisica(uint32_t direccion_logica);

#endif
