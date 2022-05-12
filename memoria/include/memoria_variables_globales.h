#ifndef MEMORIA_VARIABLES_GLOBALES_H
#define MEMORIA_VARIABLES_GLOBALES_H

// Sin esto no compila! (no se puede #includear a los .h de memoria)
struct t_memoria_config;
struct t_memoria_ram;

#include <commons/log.h>

extern struct t_memoria_config *config;
extern struct t_memoria_ram *memoria_ram;
extern t_log *logger;

#endif
