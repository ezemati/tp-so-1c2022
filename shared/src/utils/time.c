#include <utils/time.h>

double current_time()
{
    struct timeval time;
    gettimeofday(&time, NULL);

    return time.tv_usec;

    /*
    time_t rawtime = time(NULL);
    struct tm timeinfo = *localtime(&rawtime);
    time_t localtime = mktime(&timeinfo);
    return localtime;
    */
}

double milisegundos_entre_times(double t1, double t2)
{
    return t2 - t1;
}

bool times_son_iguales(double t1, double t2)
{
    return comparar_times(t1, t2) == 0;
}

int comparar_times(double t1, double t2)
{
    return t1 - t2;
}
