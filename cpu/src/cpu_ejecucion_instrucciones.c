#include <cpu_ejecucion_instrucciones.h>

void ejecutar_instruccion(t_instruccion *instruccion, uint32_t valor_para_copy)
{
    switch (instruccion->codigo_instruccion)
    {
    case NO_OP:
        ejecutar_noop(instruccion);
        break;
    case IO:
        ejecutar_io(instruccion);
        break;
    case READ:
        ejecutar_read(instruccion);
        break;
    case WRITE:
        ejecutar_write(instruccion);
        break;
    case COPY:
        ejecutar_copy(instruccion, valor_para_copy);
        break;
    case EXIT:
        ejecutar_exit(instruccion);
        break;
    }
}

void ejecutar_noop(t_instruccion *instruccion)
{
    double milisegundos_noop = config->retardo_noop;
    log_info_if_logger_not_null(logger, "PID %d: empezando NO-OP por %f milisegundos", info_ejecucion_actual->pid, milisegundos_noop);

    double microsegundos_noop = milisegundos_a_microsegundos(config->retardo_noop);
    usleep(microsegundos_noop);

    log_info_if_logger_not_null(logger, "PID %d: NO-OP finalizado", info_ejecucion_actual->pid);
}

void ejecutar_io(t_instruccion *instruccion)
{
    // TODO
    uint32_t duracion_io = instruccion->parametros[0];
    log_info_if_logger_not_null(logger, "PID %d: IO por %d milisegundos", info_ejecucion_actual->pid, duracion_io);
}

void ejecutar_read(t_instruccion *instruccion)
{
    // TODO
    uint32_t direccion_logica_lectura = instruccion->parametros[0];
    log_info_if_logger_not_null(logger, "PID %d: READ de direccion logica %d", info_ejecucion_actual->pid, direccion_logica_lectura);
}

void ejecutar_write(t_instruccion *instruccion)
{
    // TODO
    uint32_t direccion_logica_escritura = instruccion->parametros[0];
    uint32_t valor_a_escribir = instruccion->parametros[1];
    log_info_if_logger_not_null(logger, "PID %d: WRITE de valor %d a direccion logica %d", info_ejecucion_actual->pid, valor_a_escribir, direccion_logica_escritura);
}

void ejecutar_copy(t_instruccion *instruccion, uint32_t valor_a_copiar)
{
    // TODO
    uint32_t direccion_logica_destino = instruccion->parametros[0];
    uint32_t direccion_logica_origen = instruccion->parametros[1];
    log_info_if_logger_not_null(logger, "PID %d: COPY de direccion logica %d a direccion logica %d (valor: %d)", info_ejecucion_actual->pid, direccion_logica_origen, direccion_logica_destino, valor_a_copiar);
}

void ejecutar_exit(t_instruccion *instruccion)
{
    // TODO
    log_info_if_logger_not_null(logger, "PID %d: EXIT", info_ejecucion_actual->pid);
}
