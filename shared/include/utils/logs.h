#ifndef UTILS_LOGS_H
#define UTILS_LOGS_H

#include <commons/log.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

/**
 * @DESC: Llama a log_info() en caso de que el parametro logger no sea NULL
 */
void log_info_if_logger_not_null(t_log *logger, char *format, ...);

/**
 * @DESC: Llama a log_error() en caso de que el parametro logger no sea NULL
 */
void log_error_if_logger_not_null(t_log *logger, char *format, ...);

#endif