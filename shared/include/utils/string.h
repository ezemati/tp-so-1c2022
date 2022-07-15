#ifndef UTILS_STRING_H
#define UTILS_STRING_H

#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *int_to_string(uint32_t number);
long string_to_long(char *string);
void my_string_trim(char **string);

#endif
