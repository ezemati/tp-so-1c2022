#ifndef UTILS_LIST_H
#define UTILS_LIST_H

#include <commons/collections/list.h>

#include <stdlib.h>
#include <stdio.h>

void *list_get_first_element(t_list *list);
void *list_get_last_element(t_list *list);
void *list_get_minimum_or_null_if_empty(t_list *list, void *(*minimum)(void *, void *));
int list_get_last_index(t_list *list);
void list_add_in_index_custom(t_list *list, int index, void *data);
t_link_element* list_create_element(void* data);

#endif
