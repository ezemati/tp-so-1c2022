#include <utils/list.h>

void *list_get_last_element(t_list *list)
{
    t_list_iterator *iterator = list_iterator_create(list);

    void *element = NULL;
    while (list_iterator_has_next(iterator))
    {
        element = list_iterator_next(iterator);
    }

    return element;
}
