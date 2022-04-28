#include <types/inicializarproceso_request.h>

t_inicializarproceso_request *inicializarproceso_request_new(uint32_t pid, uint32_t tamanio_proceso)
{
    t_inicializarproceso_request *request = malloc(sizeof(t_inicializarproceso_request));
    request->pid = pid;
    request->tamanio_proceso = tamanio_proceso;
    return request;
}

void inicializarproceso_request_destroy(t_inicializarproceso_request *request)
{
    free(request);
}
