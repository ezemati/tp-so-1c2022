#ifndef UTILS_TIME_H
#define UTILS_TIME_H

#include <utils/converter.h>
#include <math.h>
#include <time.h>

time_t current_time();
double milisegundos_entre_times(time_t t1, time_t t2);
bool times_son_iguales(time_t t1, time_t t2);

#endif
