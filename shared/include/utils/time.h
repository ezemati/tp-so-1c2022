#ifndef UTILS_TIME_H
#define UTILS_TIME_H

#include <types/time_types.h>
#include <utils/converter.h>

#include <math.h>
#include <time.h>
#include <sys/time.h>

time_miliseg current_time();

/**
 * @brief Devuelve [t2 - t1] (tFinal - tInicial)
 */
time_miliseg milisegundos_entre_times(time_miliseg t1, time_miliseg t2);

bool times_son_iguales(time_miliseg t1, time_miliseg t2);

/**
 * @brief Compara dos time_miliseg y devuelve un entero cuyo signo indica cual es mayor
 * @return Si es positivo, t1 es mayor a t2. Si es negativo, t2 es mayor a t1. Si es
 * 0, son iguales.
 */
int comparar_times(time_miliseg t1, time_miliseg t2);

#endif
