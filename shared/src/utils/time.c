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
