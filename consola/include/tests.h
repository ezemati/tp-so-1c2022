#ifndef TESTS_H
#define TESTS_H

#include "utils.h"

#include <stdlib.h>
#include <stdint.h>
#include <CUnit/Basic.h>

int run_tests();
void serializar_deserializar_instrucciones();
void assert_instruccion_serializada(void *serializado, t_instruccion *instruccion, int *desplazamiento);

#endif
