#include <utils/converter.h>

time_microseg milisegundos_a_microsegundos(time_miliseg milisegundos)
{
    return milisegundos * 1000;
}

time_miliseg microsegundos_a_milisegundos(time_microseg microsegundos)
{
    return microsegundos / 1000;
}

time_nanoseg milisegundos_a_nanosegundos(time_miliseg milisegundos)
{
    return milisegundos * 1000000;
}

time_miliseg segundos_a_milisegundos(time_seg segundos)
{
    return segundos * 1000;
}
