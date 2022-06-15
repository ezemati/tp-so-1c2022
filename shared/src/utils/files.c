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

uint32_t file_size(FILE *file)
{
    long current_pos = ftell(file);

    fseek(file, 0L, SEEK_END);
    uint32_t size = ftell(file);

    fseek(file, current_pos, SEEK_SET);

    return size;
}
