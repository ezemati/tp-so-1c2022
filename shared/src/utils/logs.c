#include <utils/logs.h>

static void log_if_logger_not_null(t_log *logger, t_log_level log_level, char *format, va_list args);

void log_info_if_logger_not_null(t_log *logger, char *format, ...)
{
    va_list args;
    va_start(args, format);

    log_if_logger_not_null(logger, LOG_LEVEL_INFO, format, args);

    va_end(args);
}

void log_trace_if_logger_not_null(t_log *logger, char *format, ...)
{
    va_list args;
    va_start(args, format);

    log_if_logger_not_null(logger, LOG_LEVEL_TRACE, format, args);

    va_end(args);
}

void log_error_if_logger_not_null(t_log *logger, char *format, ...)
{
    va_list args;
    va_start(args, format);

    log_if_logger_not_null(logger, LOG_LEVEL_ERROR, format, args);

    va_end(args);
}

void log_if_logger_not_null(t_log *logger, t_log_level log_level, char *format, va_list args)
{
    if (logger == NULL)
    {
        return;
    }

    int max_length = 200; // Deberia ser lo suficientemente grande para todos los mensajes, agrandar si es necesario
    char *message = malloc(sizeof(char) * 200);
    vsnprintf(message, max_length, format, args);

    switch (log_level)
    {
    case LOG_LEVEL_TRACE:
        log_trace(logger, "%s", message);
        break;
    case LOG_LEVEL_DEBUG:
        log_debug(logger, "%s", message);
        break;
    case LOG_LEVEL_INFO:
        log_info(logger, "%s", message);
        break;
    case LOG_LEVEL_WARNING:
        log_warning(logger, "%s", message);
        break;
    case LOG_LEVEL_ERROR:
        log_error(logger, "%s", message);
        break;
    default:
        break;
    }

    free(message);
}
