#include <utils/logs.h>

static void log_if_logger_not_null(t_log *logger, t_log_level log_level, char *format, va_list args);
static void log_with_mutex(t_log *logger, pthread_mutex_t *mutex, t_log_level log_level, char *format, va_list args);
static int count_files_in_directory(char *path);

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

void log_trace_with_mutex(t_log *logger, pthread_mutex_t *mutex, char *format, ...)
{
    va_list args;
    va_start(args, format);

    log_with_mutex(logger, mutex, LOG_LEVEL_TRACE, format, args);

    va_end(args);
}

void log_debug_with_mutex(t_log *logger, pthread_mutex_t *mutex, char *format, ...)
{
    va_list args;
    va_start(args, format);

    log_with_mutex(logger, mutex, LOG_LEVEL_DEBUG, format, args);

    va_end(args);
}

void log_info_with_mutex(t_log *logger, pthread_mutex_t *mutex, char *format, ...)
{
    va_list args;
    va_start(args, format);

    log_with_mutex(logger, mutex, LOG_LEVEL_INFO, format, args);

    va_end(args);
}

void log_warning_with_mutex(t_log *logger, pthread_mutex_t *mutex, char *format, ...)
{
    va_list args;
    va_start(args, format);

    log_with_mutex(logger, mutex, LOG_LEVEL_WARNING, format, args);

    va_end(args);
}

void log_error_with_mutex(t_log *logger, pthread_mutex_t *mutex, char *format, ...)
{
    va_list args;
    va_start(args, format);

    log_with_mutex(logger, mutex, LOG_LEVEL_ERROR, format, args);

    va_end(args);
}

char *get_log_file_name(char *module)
{
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));

    char *folder_path = string_from_format("%s/logs", cwd);
    int files_in_log_folder = count_files_in_directory(folder_path);
    free(folder_path);

    // Por ejemplo cpu_01.log, cpu_02.log, ...
    char *relative_file_name = string_from_format("logs/%s_%02d.log", module, files_in_log_folder);
    return relative_file_name;
}

static void log_if_logger_not_null(t_log *logger, t_log_level log_level, char *format, va_list args)
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

static void log_with_mutex(t_log *logger, pthread_mutex_t *mutex, t_log_level log_level, char *format, va_list args)
{
    if (mutex != NULL)
        pthread_mutex_lock(mutex);

    log_if_logger_not_null(logger, log_level, format, args);

    if (mutex != NULL)
        pthread_mutex_unlock(mutex);
}

static int count_files_in_directory(char *path)
{
    int file_count = 0;
    struct dirent *entry;

    DIR *dirp = opendir(path);
    while ((entry = readdir(dirp)) != NULL)
    {
        if (entry->d_type == DT_REG)
        {
            /* If the entry is a regular file */
            file_count++;
        }
    }
    closedir(dirp);

    return file_count;
}
