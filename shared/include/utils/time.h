#ifndef UTILS_TIME_H
#define UTILS_TIME_H

#include <utils/converter.h>
#include <math.h>
#include <time.h>

time_t current_time();
double milisegundos_entre_times(time_t t1, time_t t2);
bool times_son_iguales(time_t t1, time_t t2);

/**
 * @brief Compara dos time_t y devuelve un entero cuyo signo indica cual es mayor
 * @return Si es positivo, t1 es mayor a t2. Si es negativo, t2 es mayor a t1. Si es
 * 0, son iguales.
 */
int comparar_times(time_t t1, time_t t2);

#endif
