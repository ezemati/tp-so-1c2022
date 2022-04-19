#include <serialization/programa.h>

void *serializar_programa(t_programa *programa, int *bytes)
{
    // Primero se manda un uint32 para indicar el tamanio total del paquete (asi al deserializar sabemos cuantos bytes leer)

    uint32_t bytes_buffer_sin_tamanio = bytes_totales_programa_serializado(programa);
    (*bytes) = sizeof(uint32_t) + bytes_buffer_sin_tamanio;
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    escribir_uint32(buffer, &desplazamiento, bytes_buffer_sin_tamanio);

    escribir_uint32(buffer, &desplazamiento, programa->tamanio);

    uint32_t cant_instrucciones = programa_cantidad_instrucciones(programa);
    escribir_uint32(buffer, &desplazamiento, cant_instrucciones);

    t_list_iterator *iterator = list_iterator_create(programa->instrucciones);
    while (list_iterator_has_next(iterator))
    {
        t_instruccion *instruccion = (t_instruccion *)list_iterator_next(iterator);
        int bytes_instruccion = 0;
        void *buffer_instruccion = serializar_instruccion(instruccion, &bytes_instruccion);
        escribir_buffer(buffer, &desplazamiento, buffer_instruccion, bytes_instruccion);
        free(buffer_instruccion);
    }

    list_iterator_destroy(iterator);

    return buffer;
}

t_programa *deserializar_programa(void *buffer)
{
    int desplazamiento = 0;

    uint32_t bytes_programa_serializado = leer_uint32(buffer, &desplazamiento);

    uint32_t tamanio = leer_uint32(buffer, &desplazamiento);

    uint32_t cant_instrucciones = leer_uint32(buffer, &desplazamiento);

    t_list *instrucciones = list_create();
    for (int i = 0; i < cant_instrucciones; i++)
    {
        uint32_t bytes_instruccion_serializada;
        t_instruccion *instruccion = deserializar_instruccion(buffer, &desplazamiento, &bytes_instruccion_serializada);
        list_add(instrucciones, instruccion);
    }

    t_programa *programa = programa_new_with_instructions(instrucciones, tamanio);
    return programa;
}

int bytes_totales_programa_serializado(t_programa *programa)
{
    /*
        TAM_PROGRAMA (uint32), CANT_INSTRUCCIONES (uint32), { TAM_INSTRUCCION (uint32), TAM_CODIGO (uint32), CODIGO (TAM_CODIGO), CANT_PARAM (uint32)[, PARAM1 (uint32)[, PARAM2(uint32)]] }
    */
    int bytes = 0;

    bytes += sizeof(programa->tamanio);
    bytes += sizeof(programa_cantidad_instrucciones(programa));

    t_list_iterator *iterator = list_iterator_create(programa->instrucciones);
    while (list_iterator_has_next(iterator))
    {
        t_instruccion *instruccion = (t_instruccion *)list_iterator_next(iterator);
        int bytes_instruccion = bytes_totales_instruccion_serializada(instruccion);
        bytes += sizeof(uint32_t);
        bytes += bytes_instruccion;
    }

    list_iterator_destroy(iterator);

    return bytes;
}
