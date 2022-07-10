#ifndef UTILS_SINCRO_H
#define UTILS_SINCRO_H

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void sincro_set_uint32(uint32_t *var, uint32_t new_value, pthread_mutex_t *mutex);
void sincro_set_int32(int32_t *var, int32_t new_value, pthread_mutex_t *mutex);
void sincro_set_bool(bool *var, bool new_value, pthread_mutex_t *mutex);
bool sincro_test_bool(bool *var, pthread_mutex_t *mutex);

#endif
