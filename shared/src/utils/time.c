#include <utils/time.h>

time_t current_time()
{
    time_t rawtime = time(NULL);
    return rawtime;

    /*
    time_t rawtime = time(NULL);
    struct tm timeinfo = *localtime(&rawtime);
    time_t localtime = mktime(&timeinfo);
    return localtime;
    */
}

double milisegundos_entre_times(time_t t1, time_t t2)
{
    return segundos_a_milisegundos(fabs(difftime(t1, t2)));
}
