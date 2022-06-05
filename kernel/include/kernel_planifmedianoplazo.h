#ifndef KERNEL_PLANIFICADOR_MEDIANO_PLAZO_H
#define KERNEL_PLANIFICADOR_MEDIANO_PLAZO_H

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

bool mediano_plazo_intentar_pasar_proceso_a_memoria();
void suspender_proceso(t_kernel_pcb *pcb, bool *se_paso_proceso_a_memoria);
void agregar_proceso_a_suspended_ready(t_kernel_pcb *pcb);

#endif
