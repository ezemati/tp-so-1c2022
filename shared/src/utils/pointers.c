#include <utils/pointers.h>

void free_if_not_null(void *pointer)
{
    if (pointer != NULL)
    {
        free(pointer);
    }
}
