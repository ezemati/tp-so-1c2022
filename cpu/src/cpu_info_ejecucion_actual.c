#include <cpu_info_ejecucion_actual.h>

t_cpu_info_ejecucion_actual *infoejecucionactual_new(t_cpu_ejecutarproceso_request *request)
{
    t_cpu_info_ejecucion_actual *info_ejecucion_actual = malloc(sizeof(t_cpu_info_ejecucion_actual));
    info_ejecucion_actual->pid = request->pid;
    info_ejecucion_actual->program_counter = request->program_counter;
    info_ejecucion_actual->lista_instrucciones = instrucciones_duplicate(request->lista_instrucciones);
    info_ejecucion_actual->time_inicio_running = current_time();
    return info_ejecucion_actual;
}

void infoejecucionactual_destroy(t_cpu_info_ejecucion_actual *info_ejecucion_actual)
{
    instrucciones_destroy(info_ejecucion_actual->lista_instrucciones);
    free(info_ejecucion_actual);
}

bool ejecucion_completada(t_cpu_info_ejecucion_actual *self)
{
    // Si tiene 2 instrucciones, entonces termina cuando el ProgramCounter es == 2
    return self->program_counter >= list_size(self->lista_instrucciones);
}
