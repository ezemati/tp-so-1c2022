#include <serialization/serialization_utils.h>

uint32_t leer_uint32_de_buffer(void *buffer, int *desplazamiento)
{
    uint32_t valor;
    memcpy(&valor, buffer + *desplazamiento, sizeof(uint32_t));
    (*desplazamiento) += sizeof(uint32_t);
    return valor;
}

int32_t leer_int32_de_buffer(void *buffer, int *desplazamiento)
{
    int32_t valor;
    memcpy(&valor, buffer + *desplazamiento, sizeof(int32_t));
    (*desplazamiento) += sizeof(int32_t);
    return valor;
}

bool leer_bool_de_buffer(void *buffer, int *desplazamiento)
{
    bool valor;
    memcpy(&valor, buffer + *desplazamiento, sizeof(bool));
    (*desplazamiento) += sizeof(bool);
    return valor;
}

char *leer_string_de_buffer(void *buffer, uint32_t len, int *desplazamiento)
{
    char *valor = malloc(len);
    memcpy(valor, buffer + *desplazamiento, len);
    (*desplazamiento) += len;
    return valor;
}

char *leer_string_con_longitud_de_buffer(void *buffer, int *desplazamiento)
{
    uint32_t len = leer_uint32_de_buffer(buffer, desplazamiento);
    return leer_string_de_buffer(buffer, len, desplazamiento);
}

void *leer_stream_de_buffer(void *buffer, uint32_t len, int *desplazamiento)
{
    void *stream = malloc(len);
    memcpy(stream, buffer + *desplazamiento, len);
    (*desplazamiento) += len;
    return stream;
}

void leer_stream_de_buffer_noalloc(void *buffer, void *buffer_dest, uint32_t len, int *desplazamiento)
{
    memcpy(buffer_dest, buffer + *desplazamiento, len);
    (*desplazamiento) += len;
}

void escribir_uint32_en_buffer(void *buffer, int *desplazamiento, uint32_t valor)
{
    memcpy(buffer + *desplazamiento, &valor, sizeof(uint32_t));
    (*desplazamiento) += sizeof(uint32_t);
}

void escribir_int32_en_buffer(void *buffer, int *desplazamiento, int32_t valor)
{
    memcpy(buffer + *desplazamiento, &valor, sizeof(int32_t));
    (*desplazamiento) += sizeof(int32_t);
}

void escribir_bool_en_buffer(void *buffer, int *desplazamiento, bool valor)
{
    memcpy(buffer + *desplazamiento, &valor, sizeof(bool));
    (*desplazamiento) += sizeof(bool);
}

void escribir_string_con_longitud_en_buffer(void *buffer, int *desplazamiento, char *string)
{
    uint32_t len = strlen(string) + 1;
    escribir_uint32_en_buffer(buffer, desplazamiento, len);
    escribir_en_buffer(buffer, desplazamiento, string, len);
}

void escribir_en_buffer(void *buffer, int *desplazamiento, void *buffer_src, uint32_t len_buffer_src)
{
    memcpy(buffer + *desplazamiento, buffer_src, len_buffer_src);
    (*desplazamiento) += len_buffer_src;
}
