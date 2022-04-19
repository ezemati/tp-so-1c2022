#ifndef CONSOLA_CONFIG_H
#define CONSOLA_CONFIG_H

#include <stdint.h>
#include <stdlib.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/string.h>
#include <utils/logs.h>

typedef struct t_consola_config
{
    char *ruta_programa;
    uint32_t tamanio_programa;
    char *ip_kernel;
    char *puerto_kernel;
} t_consola_config;

/**
 * @DESC: Crea un puntero a una estructura t_consola_config
 */
t_consola_config *consola_config_new(char **argv, char *config_path, t_log *logger);

/**
 * @DESC: Libera los recursos de un t_consola_config
 */
void consola_config_destroy(t_consola_config *config);

#endif