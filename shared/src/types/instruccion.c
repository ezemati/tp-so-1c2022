#include <types/instruccion.h>

t_instruccion *instruccion_new(char *codigo_instruccion, char **parametros)
{
    uint32_t cantidad_parametros = 0;

    if (string_equals_ignore_case(codigo_instruccion, "NO_OP") || string_equals_ignore_case(codigo_instruccion, "EXIT"))
    {
        cantidad_parametros = 0;
    }
    else if (string_equals_ignore_case(codigo_instruccion, "I/O") || string_equals_ignore_case(codigo_instruccion, "READ"))
    {
        cantidad_parametros = 1;
    }
    else if (string_equals_ignore_case(codigo_instruccion, "WRITE") || string_equals_ignore_case(codigo_instruccion, "COPY"))
    {
        cantidad_parametros = 2;
    }

    uint32_t num_parametros[2];
    for (int i = 0; i < cantidad_parametros; i++)
    {
        num_parametros[i] = strtoul(parametros[i], NULL, 10);
    }

    return instruccion_new_with_numeric_params(string_duplicate(codigo_instruccion), cantidad_parametros, num_parametros);
}

t_instruccion *instruccion_new_with_numeric_params(char *codigo_instruccion, uint32_t cant_parametros, uint32_t *parametros)
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
    return instruccion_new_with_numeric_params(string_duplicate(instruccion->codigo_instruccion), instruccion->cantidad_parametros, instruccion->parametros);
}

void instruccion_destroy(t_instruccion *instruccion)
{
    free(instruccion->codigo_instruccion);
    free(instruccion);
}

void instrucciones_destroy(t_list *instrucciones)
{
    list_destroy_and_destroy_elements(instrucciones, (void *)instruccion_destroy);
}

/***************************** SERIALIZACION *****************************/

void *serializar_instruccion(t_instruccion *instruccion, int *bytes)
{
    // TAM_CODIGO (uint32), CODIGO (TAM_CODIGO), CANT_PARAM (uint32)[, PARAM1 (uint32)[, PARAM2(uint32)]]
    uint32_t bytes_buffer_sin_tamanio = bytes_totales_instruccion_serializada(instruccion);
    (*bytes) = bytes_buffer_sin_tamanio;
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    escribir_string_con_longitud(buffer, &desplazamiento, instruccion->codigo_instruccion);

    escribir_uint32(buffer, &desplazamiento, instruccion->cantidad_parametros);
    for (int i = 0; i < instruccion->cantidad_parametros; i++)
    {
        escribir_uint32(buffer, &desplazamiento, instruccion->parametros[i]);
    }

    return buffer;
}

t_instruccion *deserializar_instruccion(void *buffer, int *desplazamiento)
{
    char *codigo_instruccion = leer_string_con_longitud(buffer, desplazamiento);

    uint32_t cantidad_parametros = leer_uint32(buffer, desplazamiento);
    uint32_t parametros[2];
    for (int i = 0; i < cantidad_parametros; i++)
    {
        parametros[i] = leer_uint32(buffer, desplazamiento);
    }

    return instruccion_new_with_numeric_params(codigo_instruccion, cantidad_parametros, parametros);
}

int bytes_totales_instruccion_serializada(t_instruccion *instruccion)
{
    // TAM_CODIGO (uint32), CODIGO (TAM_CODIGO), CANT_PARAM (uint32)[, PARAM1 (uint32)[, PARAM2(uint32)]]
    int bytes = 0;

    uint32_t tamanio_codigo = strlen(instruccion->codigo_instruccion) + 1;
    bytes += sizeof(tamanio_codigo);
    bytes += tamanio_codigo;

    uint32_t cant_parametros = instruccion->cantidad_parametros;
    bytes += sizeof(cant_parametros);                   // Cantidad de parametros
    bytes += sizeof(cant_parametros) * cant_parametros; // Cada parametro

    return bytes;
}
