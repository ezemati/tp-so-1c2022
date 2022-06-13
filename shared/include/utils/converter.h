#ifndef UTILS_CONVERTER_H
#define UTILS_CONVERTER_H

#include <types/time_types.h>

#include <stdio.h>
#include <stdlib.h>
#include <commons/collections/list.h>

time_microseg milisegundos_a_microsegundos(time_miliseg milisegundos);
time_miliseg microsegundos_a_milisegundos(time_microseg microsegundos);
time_nanoseg milisegundos_a_nanosegundos(time_miliseg milisegundos);
time_miliseg segundos_a_milisegundos(time_seg segundos);

#endif
