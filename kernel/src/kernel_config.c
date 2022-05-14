#include <kernel_config.h>

t_kernel_config *kernel_config_new(char *config_path, t_log *logger)
{
    t_kernel_config *kernel_config = malloc(sizeof(t_kernel_config));

    t_config *config = config_create("cfg/kernel.config");

    kernel_config->ip_memoria = string_duplicate(config_get_string_value(config, "IP_MEMORIA"));
    kernel_config->puerto_memoria = config_get_int_value(config, "PUERTO_MEMORIA");
    kernel_config->ip_cpu = string_duplicate(config_get_string_value(config, "IP_CPU"));
    kernel_config->puerto_cpu_dispatch = config_get_int_value(config, "PUERTO_CPU_DISPATCH");
    kernel_config->puerto_cpu_interrupt = config_get_int_value(config, "PUERTO_CPU_INTERRUPT");
    kernel_config->puerto_escucha = config_get_int_value(config, "PUERTO_ESCUCHA");
    kernel_config->algoritmo_planificacion = string_duplicate(config_get_string_value(config, "ALGORITMO_PLANIFICACION"));
    kernel_config->estimacion_inicial = config_get_int_value(config, "ESTIMACION_INICIAL");
    kernel_config->alfa = config_get_double_value(config, "ALFA");
    kernel_config->grado_multiprogramacion = config_get_int_value(config, "GRADO_MULTIPROGRAMACION");
    kernel_config->tiempo_maximo_bloqueado = config_get_int_value(config, "TIEMPO_MAXIMO_BLOQUEADO");

    config_destroy(config);

    return kernel_config;
}

void kernel_config_destroy(t_kernel_config *config)
{
    free(config->ip_memoria);
    free(config->ip_cpu);
    free(config->algoritmo_planificacion);
    free(config);
}
