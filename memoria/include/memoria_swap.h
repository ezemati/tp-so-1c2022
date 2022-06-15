#ifndef MEMORIA_SWAP_H
#define MEMORIA_SWAP_H

#include <memoria_utils.h>
#include <memoria_variables_globales.h>

#include <utils/converter.h>
#include <utils/files.h>

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <commons/txt.h>

void swap_crear_archivo(uint32_t pid, uint32_t tamanio_proceso);
void *swap_leer_pagina(uint32_t pid, uint32_t numero_pagina);
void swap_escribir_pagina(uint32_t pid, uint32_t numero_pagina, void *contenido_pagina);
void swap_borrar_archivo(uint32_t pid);

#endif
