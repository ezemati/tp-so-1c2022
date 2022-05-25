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
    char *ip_kernel;
    uint32_t puerto_kernel;
} t_consola_config;

/**
 * @brief Crea un puntero a una estructura t_consola_config
 */
t_consola_config *consola_config_new(char *config_path, t_log *logger);

/**
 * @brief Libera los recursos de un t_consola_config
 */
void consola_config_destroy(t_consola_config *config);

#endif
