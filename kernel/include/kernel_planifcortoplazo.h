#ifndef KERNEL_PLANIFICADOR_CORTO_PLAZO_H
#define KERNEL_PLANIFICADOR_CORTO_PLAZO_H

#include <kernel_config.h>
#include <kernel_utils.h>
#include <kernel_pcb.h>
#include <kernel_variables_globales.h>

#include <utils/sockets.h>
#include <utils/converter.h>
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

void agregar_proceso_a_ready(t_kernel_pcb *pcb);
void agregar_proceso_a_ready_en_anteultima_posicion_sin_replanificar(t_kernel_pcb *pcb);
void replanificar();

#endif
