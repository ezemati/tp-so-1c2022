#include <serialization/serialization_utils.h>

uint32_t leer_uint32(void *buffer, int *desplazamiento)
{
    uint32_t valor;
    memcpy(&valor, buffer + *desplazamiento, sizeof(uint32_t));
    (*desplazamiento) += sizeof(uint32_t);
    return valor;
}

char *leer_string(void *buffer, uint32_t len, int *desplazamiento)
{
    char *valor = malloc(len);
    memcpy(valor, buffer + *desplazamiento, len);
    (*desplazamiento) += len;
    return valor;
}

char *leer_string_con_longitud(void *buffer, int *desplazamiento)
{
    uint32_t len = leer_uint32(buffer, desplazamiento);
    return leer_string(buffer, len, desplazamiento);
}

void escribir_uint32(void *buffer, int *desplazamiento, uint32_t valor)
{
    memcpy(buffer + *desplazamiento, &valor, sizeof(uint32_t));
    (*desplazamiento) += sizeof(uint32_t);
}

void escribir_string_con_longitud(void *buffer, int *desplazamiento, char *string)
{
    uint32_t len = strlen(string) + 1;
    escribir_uint32(buffer, desplazamiento, len);
    escribir_buffer(buffer, desplazamiento, string, len);
}

void escribir_buffer(void *buffer, int *desplazamiento, void *buffer_src, uint32_t len_buffer_src)
{
    memcpy(buffer + *desplazamiento, buffer_src, len_buffer_src);
    (*desplazamiento) += len_buffer_src;
}
