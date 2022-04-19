#ifndef SERIALIZATION_INSTRUCCION_H
#define SERIALIZATION_INSTRUCCION_H

#include <types/instruccion.h>
#include <serialization/serialization_utils.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <commons/collections/list.h>

void *serializar_instruccion(t_instruccion *instruccion, int *bytes);
t_instruccion *deserializar_instruccion(void *buffer, int *desplazamiento, uint32_t *bytes_instruccion_serializada);
int bytes_totales_instruccion_serializada(t_instruccion *instruccion);

#endif
