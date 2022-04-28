#include <memoria_ram.h>

t_memoria_ram *memoria_ram_new()
{
    t_memoria_ram *memoria = malloc(sizeof(t_memoria_ram));
    memoria->memoria_usuario = malloc(config->tamanio_memoria);
    memoria->tablas_primer_nivel = list_create();
    memoria->tablas_segundo_nivel = list_create();
    return memoria;
}

void memoria_ram_destroy(t_memoria_ram *memoria)
{
    free(memoria->memoria_usuario);

    list_destroy_and_destroy_elements(memoria->tablas_primer_nivel, (void *)tabla_primernivel_destroy);
    free(memoria->tablas_primer_nivel);

    list_destroy_and_destroy_elements(memoria->tablas_segundo_nivel, (void *)tabla_segundonivel_destroy);
    free(memoria->tablas_segundo_nivel);

    free(memoria);
}
