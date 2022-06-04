#include <utils/list.h>

void *list_get_first_element(t_list *list)
{
    return list_size(list) > 0
               ? list_get(list, 0)
               : NULL;
}

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

void *list_get_minimum_or_null_if_empty(t_list *list, void *(*minimum)(void *, void *))
{
    return list_size(list) > 0
               ? list_get_minimum(list, minimum)
               : NULL;
}
