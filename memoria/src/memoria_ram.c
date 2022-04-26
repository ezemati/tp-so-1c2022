#include <memoria_ram.h>

t_memoria_ram *memoria_ram_new()
{
    t_memoria_ram *memoria = malloc(sizeof(t_memoria_ram));
    memoria->memoria_usuario = malloc(config->tamanio_memoria);
    memoria->tablas_paginas_primer_nivel = list_create();
}

void memoria_ram_destroy(t_memoria_ram *memoria)
{
    free(memoria->memoria_usuario);

    list_destroy_and_destroy_elements(memoria->tablas_paginas_primer_nivel, tabla_paginas_primernivel_destroy_aux);
    free(memoria->tablas_paginas_primer_nivel);

    free(memoria);
}
