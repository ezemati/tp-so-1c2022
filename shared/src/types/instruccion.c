#include <types/instruccion.h>

t_instruccion *instruccion_new(char *codigo_instruccion, char **parametros)
{
    t_instruccion *instruccion = malloc(sizeof(t_instruccion));
    instruccion->codigo_instruccion = string_duplicate(codigo_instruccion);

    if (string_equals_ignore_case(instruccion->codigo_instruccion, "NO_OP") || string_equals_ignore_case(instruccion->codigo_instruccion, "EXIT"))
    {
        instruccion->cantidad_parametros = 0;
    }
    else if (string_equals_ignore_case(instruccion->codigo_instruccion, "I/O") || string_equals_ignore_case(instruccion->codigo_instruccion, "READ"))
    {
        instruccion->cantidad_parametros = 1;
    }
    else if (string_equals_ignore_case(instruccion->codigo_instruccion, "WRITE") || string_equals_ignore_case(instruccion->codigo_instruccion, "COPY"))
    {
        instruccion->cantidad_parametros = 2;
    }

    for (int i = 0; i < instruccion->cantidad_parametros; i++)
    {
        instruccion->parametros[i] = strtoul(parametros[i], NULL, 10);
    }

    return instruccion;
}

void instruccion_destroy(t_instruccion *instruccion)
{
    free(instruccion->codigo_instruccion);
    free(instruccion);
}
