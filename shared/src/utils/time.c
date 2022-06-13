#include <utils/time.h>

time_miliseg current_time()
{
    struct timeval time;
    gettimeofday(&time, NULL);

    return microsegundos_a_milisegundos(time.tv_usec);
}

time_miliseg milisegundos_entre_times(time_miliseg t1, time_miliseg t2)
{
    return t2 - t1;
}

bool times_son_iguales(time_miliseg t1, time_miliseg t2)
{
    return comparar_times(t1, t2) == 0;
}

int comparar_times(time_miliseg t1, time_miliseg t2)
{
    return t1 - t2;
}
