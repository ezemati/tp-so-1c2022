#ifndef SERIALIZATION_UTILS_H
#define SERIALIZATION_UTILS_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint32_t leer_uint32_de_buffer(void *buffer, int *desplazamiento);
int32_t leer_int32_de_buffer(void *buffer, int *desplazamiento);
bool leer_bool_de_buffer(void *buffer, int *desplazamiento);
char *leer_string_de_buffer(void *buffer, uint32_t len, int *desplazamiento);
char *leer_string_con_longitud_de_buffer(void *buffer, int *desplazamiento);
void *leer_stream_de_buffer(void *buffer, uint32_t len, int *desplazamiento);
void leer_stream_de_buffer_noalloc(void *buffer, void *buffer_dest, uint32_t len, int *desplazamiento);

void escribir_uint32_en_buffer(void *buffer, int *desplazamiento, uint32_t valor);
void escribir_int32_en_buffer(void *buffer, int *desplazamiento, int32_t valor);
void escribir_bool_en_buffer(void *buffer, int *desplazamiento, bool valor);
void escribir_string_con_longitud_en_buffer(void *buffer, int *desplazamiento, char *string);
void escribir_en_buffer(void *buffer, int *desplazamiento, void *buffer_src, uint32_t len_buffer_src);

#endif
