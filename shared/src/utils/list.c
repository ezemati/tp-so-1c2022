#include <utils/list.h>

void *list_get_last_element(t_list *list)
{
    void *element = NULL;
    t_list_iterator *iterator = list_iterator_create(list);
    while (list_iterator_has_next(iterator))
    {
        element = list_iterator_next(iterator);
    }
    list_iterator_destroy(iterator);

    return element;
}
