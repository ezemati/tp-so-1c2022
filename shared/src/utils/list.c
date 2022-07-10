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

void *list_get_minimum_or_null_if_empty_with_mutex(t_list *list, void *(*minimum)(void *, void *), pthread_mutex_t *mutex)
{
    pthread_mutex_lock(mutex);
    void *minimum_element = list_size(list) > 0
                                ? list_get_minimum(list, minimum)
                                : NULL;
    pthread_mutex_unlock(mutex);
    return minimum_element;
}

void list_add_with_mutex(t_list *list, void *element, pthread_mutex_t *mutex)
{
    pthread_mutex_lock(mutex);
    list_add(list, element);
    pthread_mutex_unlock(mutex);
}

int list_size_with_mutex(t_list *list, pthread_mutex_t *mutex)
{
    pthread_mutex_lock(mutex);
    int size = list_size(list);
    pthread_mutex_unlock(mutex);
    return size;
}

void *list_get_first_element_with_mutex(t_list *list, pthread_mutex_t *mutex)
{
    pthread_mutex_lock(mutex);
    void *result = list_size(list) > 0
                       ? list_get(list, 0)
                       : NULL;
    pthread_mutex_unlock(mutex);
    return result;
}

int list_get_last_index_with_mutex(t_list *list, pthread_mutex_t *mutex)
{
    pthread_mutex_lock(mutex);
    int last_index = list_size(list) - 1;
    pthread_mutex_unlock(mutex);
    return last_index;
}

void *list_remove_last_with_mutex(t_list *list, pthread_mutex_t *mutex)
{
    pthread_mutex_lock(mutex);
    int last_index = list_size(list) - 1;
    void *last_element = list_remove(list, last_index);
    pthread_mutex_unlock(mutex);
    return last_element;
}
