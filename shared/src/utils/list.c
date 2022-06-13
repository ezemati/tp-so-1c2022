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

int list_get_last_index(t_list *list)
{
    return list_size(list) - 1;
}

void list_add_in_index_custom(t_list *list, int index, void *data)
{
    int size = list_size(list);
    if (index == size)
    {
        // Si la lista tiene 2 elementos (indices 0 y 1) y quiero agregar al indice 2, seria agregar
        // al final de la lista
        list_add(list, data);
        return;
    }

    if (index == 0)
    {
        t_link_element *new_element = list_create_element(data);
        new_element->next = list->head;
        list->head = new_element;
    }
    else
    {
        t_link_element *previous_to_insert = list->head;
        for (int i = 0; i < index - 1; i++)
        {
            previous_to_insert = previous_to_insert->next;
        }
        t_link_element *new_element = list_create_element(data);
        new_element->next = previous_to_insert->next;
        previous_to_insert->next = new_element;
    }

    list->elements_count++;
}

t_link_element *list_create_element(void *data)
{
    t_link_element *element = malloc(sizeof(*element));
    element->data = data;
    element->next = NULL;
    return element;
}
