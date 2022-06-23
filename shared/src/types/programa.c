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
    t_programa *programa = malloc(sizeof(t_programa));
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
    free_if_not_null(programa->ruta);
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

        // Las lineas que empiezan con "//" son comentarios y no se tienen en cuenta
        if (!string_starts_with(line, "//") && !string_is_empty(line))
        {
            agregar_instruccion(line, instrucciones);
        }
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

/***************************** SERIALIZACION *****************************/

void *serializar_programa(t_programa *programa, int *bytes)
{
    (*bytes) = bytes_totales_programa_serializado(programa);
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    escribir_uint32_en_buffer(buffer, &desplazamiento, programa->tamanio);

    serializar_lista_instrucciones_en_buffer(programa->instrucciones, buffer, &desplazamiento);

    return buffer;
}

t_programa *deserializar_programa(void *buffer)
{
    int desplazamiento = 0;

    uint32_t tamanio_programa = leer_uint32_de_buffer(buffer, &desplazamiento);
    t_list *lista_instrucciones = deserializar_lista_instrucciones(buffer, &desplazamiento);

    t_programa *programa = programa_new_with_instructions(lista_instrucciones, tamanio_programa);
    return programa;
}

int bytes_totales_programa_serializado(t_programa *programa)
{
    /*
        TAM_PROGRAMA (uint32), LISTA_INSTRUCCIONES
    */
    int bytes = 0;

    bytes += sizeof(programa->tamanio);
    bytes += bytes_totales_lista_instrucciones_serializada(programa->instrucciones);

    return bytes;
}
