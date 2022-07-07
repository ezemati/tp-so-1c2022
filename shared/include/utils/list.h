#ifndef UTILS_LIST_H
#define UTILS_LIST_H

#include <commons/collections/list.h>

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

void *list_get_last_element(t_list *list);
void *list_get_minimum_or_null_if_empty(t_list *list, void *(*minimum)(void *, void *));
t_link_element *list_create_element(void *data);

void list_add_with_mutex(t_list *list, void *element, pthread_mutex_t *mutex);
void list_add_in_index_with_mutex(t_list *list, int index, void *element, pthread_mutex_t *mutex);
int list_size_with_mutex(t_list *list, pthread_mutex_t *mutex);
void *list_get_first_element_with_mutex(t_list *list, pthread_mutex_t *mutex);
int list_get_last_index_with_mutex(t_list *list, pthread_mutex_t *mutex);

#endif
