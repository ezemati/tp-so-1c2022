#include <utils/time.h>

time_miliseg current_time()
{
    // https://stackoverflow.com/a/10889489
    struct timeval tv;
    gettimeofday(&tv, NULL);

    unsigned long long millisecondsSinceEpoch =
        (unsigned long long)(tv.tv_sec) * 1000 +
        (unsigned long long)(tv.tv_usec) / 1000;

    return millisecondsSinceEpoch;
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
    return t1 > t2
               ? 1
               : (t1 < t2
                      ? -1
                      : 0);
}
