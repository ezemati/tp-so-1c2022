#ifndef UTILS_FILES_H
#define UTILS_FILES_H

#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fopen_with_format(char *mode, char *format, ...);

#endif
