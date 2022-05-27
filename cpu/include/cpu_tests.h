#ifndef CPU_TESTS_H
#define CPU_TESTS_H

#include <cpu/ejecutar_proceso.h>

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <CUnit/Basic.h>
#include <commons/collections/list.h>

int run_tests();
void serializar_deserializar_ejecutarproceso_request_response();

#endif
