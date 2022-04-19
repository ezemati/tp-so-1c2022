#ifndef CONSOLA_TESTS_H
#define CONSOLA_TESTS_H

#include <consola_utils.h>
#include <types/instruccion.h>
#include <serialization/instruccion.h>
#include <serialization/programa.h>

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <CUnit/Basic.h>
#include <commons/collections/list.h>

int run_tests();
void serializar_deserializar_instrucciones();
void assert_instruccion_serializada(void *serializado, t_instruccion *instruccion, int *desplazamiento);

#endif
