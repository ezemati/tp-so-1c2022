#include <types/instruccion.h>

static void instruccion_buffer_destroy(void *buffer_instruccion);

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

    return instruccion_new_with_numeric_params(codigo_instruccion, cantidad_parametros, num_parametros);
}

t_instruccion *instruccion_new_with_numeric_params(char *codigo_instruccion, uint32_t cant_parametros, uint32_t *parametros)
{
    t_instruccion *instruccion = malloc(sizeof(t_instruccion));
    instruccion->codigo_instruccion = string_duplicate(codigo_instruccion);
    instruccion->cantidad_parametros = cant_parametros;

    for (int i = 0; i < instruccion->cantidad_parametros; i++)
    {
        instruccion->parametros[i] = parametros[i];
    }

    return instruccion;
}

void instruccion_destroy(t_instruccion *instruccion)
{
    free(instruccion->codigo_instruccion);
    free(instruccion);
}

void instrucciones_destroy(t_list *instrucciones)
{
    list_destroy_and_destroy_elements(instrucciones, instruccion_buffer_destroy);
}

void instruccion_buffer_destroy(void *buffer_instruccion)
{
    t_instruccion *instruccion = (t_instruccion *)buffer_instruccion;
    instruccion_destroy(instruccion);
}
