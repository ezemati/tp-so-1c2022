#include <types/programa.h>

static void agregar_instruccion(char *linea, t_list *instrucciones);

t_programa *programa_new(char *ruta, uint32_t tamanio)
{
    t_list *instrucciones = crear_instrucciones_de_archivo(ruta);
    t_programa *programa = programa_new_with_instructions(instrucciones, tamanio);
    programa->ruta = string_duplicate(ruta);
    return programa;
}

t_programa *programa_new_with_instructions(t_list *instrucciones, uint32_t tamanio)
{
    t_programa *programa = malloc(sizeof(programa));
    programa->ruta = NULL;
    programa->tamanio = tamanio;
    programa->instrucciones = instrucciones;
    return programa;
}

uint32_t programa_cantidad_instrucciones(t_programa *programa)
{
    return list_size(programa->instrucciones);
}

void programa_destroy(t_programa *programa)
{
    free(programa->ruta);
    instrucciones_destroy(programa->instrucciones);
    free(programa);
}

t_list *crear_instrucciones_de_archivo(char *ruta)
{
    FILE *archivo = fopen(ruta, "r");

    t_list *instrucciones = list_create();

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, archivo)) != -1)
    {
        string_trim(&line);

        agregar_instruccion(line, instrucciones);
    }

    free(line);
    fclose(archivo);

    return instrucciones;
}

void agregar_instruccion(char *linea, t_list *instrucciones)
{
    char **linea_spliteada = string_split(linea, " ");
    char *codigo_instruccion = linea_spliteada[0];

    if (string_equals_ignore_case(codigo_instruccion, "NO_OP"))
    {
        int cantidad_noop = strtoul(linea_spliteada[1], NULL, 10);
        for (int i = 1; i <= cantidad_noop; i++)
        {
            t_instruccion *instruccion = instruccion_new(codigo_instruccion, NULL);
            list_add(instrucciones, instruccion);
        }
    }
    else
    {
        // Los parametros empiezan en la segunda posicion del linea_spliteada (por eso el `+ 1`)
        t_instruccion *instruccion = instruccion_new(codigo_instruccion, linea_spliteada + 1);
        list_add(instrucciones, instruccion);
    }

    string_array_destroy(linea_spliteada);
}
