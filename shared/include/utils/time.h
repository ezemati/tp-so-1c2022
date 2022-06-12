#ifndef UTILS_TIME_H
#define UTILS_TIME_H

#include <utils/converter.h>

#include <math.h>
#include <time.h>
#include <sys/time.h>

double current_time();

/**
 * @brief Devuelve [t2 - t1] (tFinal - tInicial)
 */
double milisegundos_entre_times(double t1, double t2);

bool times_son_iguales(double t1, double t2);

/**
 * @brief Compara dos double (tiempo en milisegundos) y devuelve un entero cuyo signo indica cual es mayor
 * @return Si es positivo, t1 es mayor a t2. Si es negativo, t2 es mayor a t1. Si es
 * 0, son iguales.
 */
int comparar_times(double t1, double t2);

#endif
