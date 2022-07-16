#ifndef UTILS_LOGS_H
#define UTILS_LOGS_H

#include <commons/log.h>
#include <commons/string.h>
#include <dirent.h>
#include <limits.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief Llama a log_info() en caso de que el parametro logger no sea NULL
 */
void log_info_if_logger_not_null(t_log *logger, char *format, ...);

/**
 * @brief Llama a log_trace() en caso de que el parametro logger no sea NULL
 */
void log_trace_if_logger_not_null(t_log *logger, char *format, ...);

/**
 * @brief Llama a log_error() en caso de que el parametro logger no sea NULL
 */
void log_error_if_logger_not_null(t_log *logger, char *format, ...);

void log_trace_with_mutex(t_log *logger, pthread_mutex_t *mutex, char *format, ...);
void log_debug_with_mutex(t_log *logger, pthread_mutex_t *mutex, char *format, ...);
void log_info_with_mutex(t_log *logger, pthread_mutex_t *mutex, char *format, ...);
void log_error_with_mutex(t_log *logger, pthread_mutex_t *mutex, char *format, ...);

char *get_log_file_name(char *module);

#endif
