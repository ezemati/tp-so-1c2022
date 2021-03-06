#ifndef KERNEL_PLANIFICADOR_LARGO_PLAZO_H
#define KERNEL_PLANIFICADOR_LARGO_PLAZO_H

#include <kernel_config.h>
#include <kernel_utils.h>
#include <kernel_pcb.h>
#include <kernel_variables_globales.h>

#include <utils/sockets.h>
#include <types/identificador_operacion.h>

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <commons/collections/list.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/txt.h>

bool largo_plazo_intentar_pasar_proceso_a_memoria();
void finalizar_proceso(t_kernel_pcb *pcb);

#endif
