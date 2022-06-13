#ifndef KERNEL_TESTS_H
#define KERNEL_TESTS_H

#include <kernel/actualizar_pcb.h>

#include <types/time_types.h>
#include <utils/time.h>

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <CUnit/Basic.h>
#include <commons/collections/list.h>

int run_tests();
void serializar_deserializar_actualizarpcb_request_response();

#endif
