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

/***************************** SERIALIZACION *****************************/

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
