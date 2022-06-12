#include <memoria_entrada_segundonivel.h>

t_entrada_segundonivel *entrada_segundonivel_new(uint32_t pid, uint32_t numero_entrada, uint32_t numero_pagina)
{
    t_entrada_segundonivel *entrada_segundonivel = malloc(sizeof(t_entrada_segundonivel));
    entrada_segundonivel->pid = pid;
    entrada_segundonivel->numero_entrada = numero_entrada;
    entrada_segundonivel->numero_pagina = numero_pagina;
    entrada_segundonivel->numero_marco = -1;
    entrada_segundonivel->bit_presencia = false;
    entrada_segundonivel->bit_modificado = false;
    entrada_segundonivel->bit_uso = true;
    return entrada_segundonivel;
}

void entrada_segundonivel_destroy(t_entrada_segundonivel *entrada_segundonivel)
{
    free(entrada_segundonivel);
}

bool entrada_segundonivel_tiene_pagina_presente(t_entrada_segundonivel *self)
{
    return self->bit_presencia;
}

uint32_t marco_de_pagina_de_entrada_segundonivel(t_entrada_segundonivel *self)
{
    if (!entrada_segundonivel_tiene_pagina_presente(self))
    {
        log_error_if_logger_not_null(logger, "Marco solicitado a una entrada de 2do nivel cuya pagina no esta presente: entrada nro %d", self->numero_entrada);
        return -1;
    }

    return self->numero_marco;
}

void entrada_segundonivel_marcar_pagina_cargada(t_entrada_segundonivel *self, uint32_t numero_marco)
{
    self->numero_marco = numero_marco;
    self->bit_presencia = true;
    self->bit_modificado = false;
    self->bit_uso = true;
}

void entrada_segundonivel_marcar_pagina_descargada(t_entrada_segundonivel *self)
{
    self->numero_marco = 131313;
    self->bit_presencia = false;
    self->bit_modificado = false;
    self->bit_uso = false;
}

bool pagina_esta_modificada(t_entrada_segundonivel *self)
{
    return self->bit_modificado;
}

void marcar_pagina_modificada(t_entrada_segundonivel *self)
{
    self->bit_modificado = true;
}

void marcar_pagina_usada(t_entrada_segundonivel *self)
{
    self->bit_uso = true;
}
