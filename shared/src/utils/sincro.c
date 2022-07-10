#include <utils/sincro.h>

void sincro_set_uint32(uint32_t *var, uint32_t new_value, pthread_mutex_t *mutex)
{
    pthread_mutex_lock(mutex);
    (*var) = new_value;
    pthread_mutex_unlock(mutex);
}

void sincro_set_int32(int32_t *var, int32_t new_value, pthread_mutex_t *mutex)
{
    pthread_mutex_lock(mutex);
    (*var) = new_value;
    pthread_mutex_unlock(mutex);
}

void sincro_set_bool(bool *var, bool new_value, pthread_mutex_t *mutex)
{
    pthread_mutex_lock(mutex);
    (*var) = new_value;
    pthread_mutex_unlock(mutex);
}

bool sincro_test_bool(bool *var, pthread_mutex_t *mutex)
{
    pthread_mutex_lock(mutex);
    bool ret = (*var);
    pthread_mutex_unlock(mutex);
    return ret;
}
