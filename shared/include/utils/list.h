#ifndef UTILS_LIST_H
#define UTILS_LIST_H

#include <commons/collections/list.h>

#include <stdlib.h>
#include <stdio.h>

void *list_get_first_element(t_list *list);
void *list_get_last_element(t_list *list);
void *list_get_minimum_or_null_if_empty(t_list *list, void *(*minimum)(void *, void *));

#endif
