#ifndef SERIALIZATION_UTILS_H
#define SERIALIZATION_UTILS_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint32_t leer_uint32(void *buffer, int *desplazamiento);
char *leer_string(void *buffer, uint32_t len, int *desplazamiento);
char *leer_string_con_longitud(void *buffer, int *desplazamiento);
void *leer_stream(void *buffer, uint32_t len, int *desplazamiento);
void escribir_uint32(void *buffer, int *desplazamiento, uint32_t valor);
void escribir_string_con_longitud(void *buffer, int *desplazamiento, char *string);
void escribir_buffer(void *buffer, int *desplazamiento, void *buffer_src, uint32_t len_buffer_src);

#endif
