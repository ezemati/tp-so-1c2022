#ifndef SERIALIZATION_PROGRAMA_H
#define SERIALIZATION_PROGRAMA_H

#include <types/programa.h>
#include <serialization/serialization_instruccion.h>
#include <serialization/serialization_utils.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <commons/collections/list.h>

void *serializar_programa(t_programa *programa, int *bytes);
int bytes_totales_programa_serializado(t_programa *programa);

#endif
