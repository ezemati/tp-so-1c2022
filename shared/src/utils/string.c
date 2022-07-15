#include <utils/string.h>

char *int_to_string(uint32_t number)
{
    char str[100];
    sprintf(str, "%d", number);

    uint32_t size = strlen(str) + 1;
    char *allocatedStr = malloc(size);
    strncpy(allocatedStr, str, size);
    return allocatedStr;
}

long string_to_long(char *string)
{
    return strtol(string, NULL, 10);
}

void my_string_trim(char **string)
{
    // Right trim
    size_t end = strlen(*string) - 1;
    while (end >= 0 && isspace((*string)[end]))
    {
        (*string)[end] = '\0';
        end--;
    }
}
