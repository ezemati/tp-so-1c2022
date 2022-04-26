#include <cpu_config.h>

t_cpu_config *cpu_config_new(char *config_path, t_log *logger)
{
    t_cpu_config *cpu_config = malloc(sizeof(t_cpu_config));

    t_config *config = config_create("cfg/cpu.config");
    cpu_config->entradas_tbl = config_get_int_value(config, "ENTRADAS_TLB");
    cpu_config->reemplazo_tbl = string_duplicate(config_get_string_value(config, "REEMPLAZO_TLB"));
    cpu_config->retardo_noop = config_get_int_value(config, "RETARDO_NOOP");
    cpu_config->ip_memoria = string_duplicate(config_get_string_value(config, "IP_MEMORIA"));
    cpu_config->puerto_memoria = string_duplicate(config_get_string_value(config, "PUERTO_MEMORIA"));
    cpu_config->puerto_escucha_dispatch = config_get_int_value(config, "PUERTO_ESCUCHA_DISPATCH");
    cpu_config->puerto_escucha_interrupt = config_get_int_value(config, "PUERTO_ESCUCHA_INTERRUPT");

    config_destroy(config);

    return cpu_config;
}

void cpu_config_destroy(t_cpu_config *config)
{
    free(config->reemplazo_tbl);
    free(config->ip_memoria);
    free(config->puerto_memoria);
    free(config);
}
