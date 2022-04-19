#include <consola_config.h>

t_consola_config *consola_config_new(char *config_path, t_log *logger)
{
    t_consola_config *cons_config = malloc(sizeof(t_consola_config));

    t_config *config = config_create("cfg/consola.config");
    cons_config->ip_kernel = string_duplicate(config_get_string_value(config, "IP_KERNEL"));
    cons_config->puerto_kernel = string_duplicate(config_get_string_value(config, "PUERTO_KERNEL"));
    config_destroy(config);

    log_info_if_logger_not_null(logger, "{ IP_KERNEL = %s, PUERTO_KERNEL = %s }", cons_config->ip_kernel, cons_config->puerto_kernel);

    return cons_config;
}

void consola_config_destroy(t_consola_config *config)
{
    free(config->ip_kernel);
    free(config->puerto_kernel);
    free(config);
}