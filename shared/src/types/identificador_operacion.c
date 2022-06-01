#include <types/identificador_operacion.h>

char *identificador_operacion_to_string(identificador_operacion id_op)
{
    switch (id_op)
    {
    case CREAR_PROCESO:
        return "CREAR_PROCESO";
    case INICIALIZAR_PROCESO:
        return "INICIALIZAR_PROCESO";
    case FINALIZAR_PROCESO:
        return "FINALIZAR_PROCESO";
    case SUSPENDER_PROCESO:
        return "SUSPENDER_PROCESO";
    case BLOQUEAR_PROCESO:
        return "BLOQUEAR_PROCESO";
    case EJECUTAR_PROCESO:
        return "EJECUTAR_PROCESO";
    case HANDSHAKE_INFO_TRADUCCION:
        return "HANDSHAKE_INFO_TRADUCCION";
    case LEER_DATO:
        return "LEER_DATO";
    case ESCRIBIR_DATO:
        return "ESCRIBIR_DATO";
    case OBTENER_NUMERO_TABLA_2_PARA_ENTRADA_TABLA_1:
        return "OBTENER_NUMERO_TABLA_2_PARA_ENTRADA_TABLA_1";
    case OBTENER_MARCO_PARA_ENTRADA_TABLA_2:
        return "OBTENER_MARCO_PARA_ENTRADA_TABLA_2";
    case HANDSHAKE_SOY_KERNEL:
        return "HANDSHAKE_SOY_KERNEL";
    default:
        return "Operacion no reconocida";
    }
}
