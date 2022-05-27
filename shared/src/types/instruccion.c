#include <types/instruccion.h>

static code_instruccion code_instruccion_from_string(char *codigo_instruccion);
static char *code_instruccion_to_string(code_instruccion codigo_instruccion);

t_instruccion *instruccion_new(char *codigo_instruccion, char **parametros)
{
    uint32_t cantidad_parametros = 0;

    code_instruccion enum_code_instruccion = code_instruccion_from_string(codigo_instruccion);

    if (enum_code_instruccion == NO_OP || enum_code_instruccion == EXIT)
    {
        cantidad_parametros = 0;
    }
    else if (enum_code_instruccion == IO || enum_code_instruccion == READ)
    {
        cantidad_parametros = 1;
    }
    else if (enum_code_instruccion == WRITE || enum_code_instruccion == COPY)
    {
        cantidad_parametros = 2;
    }

    uint32_t num_parametros[2];
    for (int i = 0; i < cantidad_parametros; i++)
    {
        num_parametros[i] = strtoul(parametros[i], NULL, 10);
    }

    return instruccion_new_with_numeric_params(enum_code_instruccion, cantidad_parametros, num_parametros);
}

t_instruccion *instruccion_new_with_numeric_params(code_instruccion codigo_instruccion, uint32_t cant_parametros, uint32_t *parametros)
{
    t_instruccion *instruccion = malloc(sizeof(t_instruccion));
    instruccion->codigo_instruccion = codigo_instruccion;
    instruccion->cantidad_parametros = cant_parametros;

    for (int i = 0; i < instruccion->cantidad_parametros; i++)
    {
        instruccion->parametros[i] = parametros[i];
    }

    return instruccion;
}

t_instruccion *instruccion_duplicate(t_instruccion *instruccion)
{
    return instruccion_new_with_numeric_params(instruccion->codigo_instruccion, instruccion->cantidad_parametros, instruccion->parametros);
}

void instruccion_destroy(t_instruccion *instruccion)
{
    free(instruccion);
}

void instrucciones_destroy(t_list *instrucciones)
{
    list_destroy_and_destroy_elements(instrucciones, (void *)instruccion_destroy);
}

char *format_instruccion_para_print(t_instruccion *instruccion)
{
    char str[100];
    snprintf(
        str,
        100,
        "%s %s %s",
        code_instruccion_to_string(instruccion->codigo_instruccion),
        instruccion->cantidad_parametros >= 1
            ? int_to_string(instruccion->parametros[0])
            : "-",
        instruccion->cantidad_parametros >= 2
            ? int_to_string(instruccion->parametros[1])
            : "-");

    int size = strlen(str) + 1;
    char *alloc_string = malloc(size);
    memcpy(alloc_string, str, size);
    return alloc_string;
}

static code_instruccion code_instruccion_from_string(char *codigo_instruccion)
{
    if (string_equals_ignore_case(codigo_instruccion, "NO_OP"))
        return NO_OP;
    if (string_equals_ignore_case(codigo_instruccion, "I/O"))
        return IO;
    if (string_equals_ignore_case(codigo_instruccion, "READ"))
        return READ;
    if (string_equals_ignore_case(codigo_instruccion, "WRITE"))
        return WRITE;
    if (string_equals_ignore_case(codigo_instruccion, "COPY"))
        return COPY;
    if (string_equals_ignore_case(codigo_instruccion, "EXIT"))
        return EXIT;

    return -1;
}

static char *code_instruccion_to_string(code_instruccion codigo_instruccion)
{
    if (codigo_instruccion == NO_OP)
        return "NO_OP";
    if (codigo_instruccion == IO)
        return "I/O";
    if (codigo_instruccion == READ)
        return "READ";
    if (codigo_instruccion == WRITE)
        return "WRITE";
    if (codigo_instruccion == COPY)
        return "COPY";
    if (codigo_instruccion == EXIT)
        return "EXIT";

    return "?";
}

/***************************** SERIALIZACION *****************************/

void *serializar_instruccion(t_instruccion *instruccion, int *bytes)
{
    // CODIGO (uint32), CANT_PARAM (uint32)[, PARAM1 (uint32)[, PARAM2(uint32)]]
    (*bytes) = bytes_totales_instruccion_serializada(instruccion);
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    escribir_uint32_en_buffer(buffer, &desplazamiento, instruccion->codigo_instruccion);

    escribir_uint32_en_buffer(buffer, &desplazamiento, instruccion->cantidad_parametros);
    for (int i = 0; i < instruccion->cantidad_parametros; i++)
    {
        escribir_uint32_en_buffer(buffer, &desplazamiento, instruccion->parametros[i]);
    }

    return buffer;
}

t_instruccion *deserializar_instruccion(void *buffer, int *desplazamiento)
{
    code_instruccion codigo_instruccion = leer_uint32_de_buffer(buffer, desplazamiento);

    uint32_t cantidad_parametros = leer_uint32_de_buffer(buffer, desplazamiento);
    uint32_t parametros[2];
    for (int i = 0; i < cantidad_parametros; i++)
    {
        parametros[i] = leer_uint32_de_buffer(buffer, desplazamiento);
    }

    return instruccion_new_with_numeric_params(codigo_instruccion, cantidad_parametros, parametros);
}

int bytes_totales_instruccion_serializada(t_instruccion *instruccion)
{
    // CODIGO (uint32), CANT_PARAM (uint32)[, PARAM1 (uint32)[, PARAM2(uint32)]]
    int bytes = 0;

    bytes += sizeof(instruccion->codigo_instruccion);

    uint32_t cant_parametros = instruccion->cantidad_parametros;
    bytes += sizeof(cant_parametros);            // Cantidad de parametros
    bytes += sizeof(uint32_t) * cant_parametros; // Cada parametro

    return bytes;
}

void *serializar_lista_instrucciones(t_list *lista_instrucciones, int *bytes)
{
    (*bytes) = bytes_totales_lista_instrucciones_serializada(lista_instrucciones);
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    serializar_lista_instrucciones_en_buffer(lista_instrucciones, buffer, &desplazamiento);

    return buffer;
}

void serializar_lista_instrucciones_en_buffer(t_list *lista_instrucciones, void *buffer, int *desplazamiento)
{
    // CANT_INSTRUCCIONES (uint32), { CODIGO (uint32), CANT_PARAM (uint32)[, PARAM1 (uint32)[, PARAM2(uint32)]] }

    uint32_t cant_instrucciones = list_size(lista_instrucciones);
    escribir_uint32_en_buffer(buffer, desplazamiento, cant_instrucciones);

    t_list_iterator *iterator = list_iterator_create(lista_instrucciones);
    while (list_iterator_has_next(iterator))
    {
        t_instruccion *instruccion = list_iterator_next(iterator);
        int bytes_instruccion = 0;
        void *buffer_instruccion = serializar_instruccion(instruccion, &bytes_instruccion);
        escribir_en_buffer(buffer, desplazamiento, buffer_instruccion, bytes_instruccion);
        free(buffer_instruccion);
    }
    list_iterator_destroy(iterator);
}

t_list *deserializar_lista_instrucciones(void *buffer, int *desplazamiento)
{
    uint32_t cantidad_instrucciones = leer_uint32_de_buffer(buffer, desplazamiento);

    t_list *lista_instrucciones = list_create();
    for (int i = 0; i < cantidad_instrucciones; i++)
    {
        t_instruccion *instruccion = deserializar_instruccion(buffer, desplazamiento);
        list_add(lista_instrucciones, instruccion);
    }

    return lista_instrucciones;
}

int bytes_totales_lista_instrucciones_serializada(t_list *lista_instrucciones)
{
    // CANT_INSTRUCCIONES (uint32), { CODIGO (uint32), CANT_PARAM (uint32)[, PARAM1 (uint32)[, PARAM2(uint32)]] }
    int bytes = 0;

    bytes += sizeof(uint32_t);

    t_list_iterator *iterator = list_iterator_create(lista_instrucciones);
    while (list_iterator_has_next(iterator))
    {
        t_instruccion *instruccion = list_iterator_next(iterator);
        int bytes_instruccion = bytes_totales_instruccion_serializada(instruccion);
        bytes += bytes_instruccion;
    }
    list_iterator_destroy(iterator);

    return bytes;
}
