#ifndef CONSOLA_H
#define CONSOLA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <commons/collections/list.h>

typedef struct t_instruccion
{
    char *codigo_instruccion;
    int cantidad_parametros;
    uint32_t parametros[2];
} t_instruccion;

void inicializar_consola(char **argv);
t_list *leer_instrucciones(FILE *archivo);
void agregar_instruccion(char *linea, t_list *instrucciones);
void instrucciones_destroy(t_list *instrucciones);
void instruccion_destroy(void *buffer_instruccion);
void terminar_consola();

#endif
