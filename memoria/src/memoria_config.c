#include <memoria_config.h>

t_memoria_config *memoria_config_new(char *config_path, t_log *logger)
{
    t_memoria_config *mem_config = malloc(sizeof(t_memoria_config));

    t_config *config = config_create(config_path);

    mem_config->puerto_escucha = config_get_int_value(config, "PUERTO_ESCUCHA");
    mem_config->tamanio_memoria = config_get_int_value(config, "TAM_MEMORIA");
    mem_config->tamanio_pagina = config_get_int_value(config, "TAM_PAGINA");
    mem_config->entradas_por_tabla = config_get_int_value(config, "ENTRADAS_POR_TABLA");
    mem_config->retardo_memoria = config_get_double_value(config, "RETARDO_MEMORIA");
    mem_config->algoritmo_reemplazo = string_duplicate(config_get_string_value(config, "ALGORITMO_REEMPLAZO"));
    mem_config->marcos_por_proceso = config_get_int_value(config, "MARCOS_POR_PROCESO");
    mem_config->retardo_swap = config_get_double_value(config, "RETARDO_SWAP");
    mem_config->path_swap = string_duplicate(config_get_string_value(config, "PATH_SWAP"));

    config_destroy(config);

    // log_info_if_logger_not_null(logger, "{ IP_KERNEL = %s, PUERTO_KERNEL = %s }", mem_config->ip_kernel, mem_config->puerto_kernel);

    return mem_config;
}

void memoria_config_destroy(t_memoria_config *config)
{
    free(config->algoritmo_reemplazo);
    free(config->path_swap);
    free(config);
}
