#include <utils/files.h>

FILE *fopen_with_format(char *mode, char *format, ...)
{
    va_list args;
    va_start(args, format);

    char file_path[PATH_MAX];
    vsprintf(file_path, format, args);

    va_end(args);

    FILE *file = fopen(file_path, mode);
    return file;
}
