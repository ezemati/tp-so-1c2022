#include <cpu/recibir_instruccion.h>

t_cpu_recibirinstruccion_response *recibirinstruccion_response_new(uint32_t id, uint32_t tamanio, void *instruccion, uint32_t program_counter,uint32_t estimacion_rafaga)
{
    t_cpu_recibirinstruccion_response *response = malloc(sizeof(t_cpu_recibirinstruccion_response));
    response->id = id;
    response->tamanio = tamanio;
    response->instruccion = instruccion;
    response->program_counter = program_counter;
    response->estimacion_rafaga = estimacion_rafaga;
    return response;
}

void recibirinstruccion_response_destroy(t_cpu_recibirinstruccion_response *response)
{
    free(response->instruccion);
    free(response);
}

/***************************** SERIALIZACION *****************************/

void *serializar_recibirinstruccion_response(t_cpu_recibirinstruccion_response *response, int *bytes)
{
    // TAM_BUFFER_RESPONSE (uint32), ID (uint32), TAMANIO (uint32), INSTRUCCION (void), PROGRAM_COUNTER (uint32), ESTIMACION_RAFAGA (uint32)
    uint32_t bytes_buffer_sin_tamanio = bytes_totales_recibirinstruccion_response_serializada(response);
    (*bytes) = sizeof(uint32_t) + bytes_buffer_sin_tamanio;
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    escribir_uint32(buffer, &desplazamiento, bytes_buffer_sin_tamanio);

    escribir_uint32(buffer, &desplazamiento, response->id);
    escribir_uint32(buffer, &desplazamiento, response->tamanio);
    escribir_string_con_longitud(buffer, &desplazamiento, response->instruccion);
    escribir_uint32(buffer, &desplazamiento, response->program_counter);
    escribir_uint32(buffer, &desplazamiento, response->estimacion_rafaga);

    return buffer;
}

t_cpu_recibirinstruccion_response *deserializar_recibirinstruccion_response(void *buffer)
{
    int desplazamiento = 0;

    uint32_t id = leer_uint32(buffer, &desplazamiento);
    uint32_t tamanio = leer_uint32(buffer, &desplazamiento);
    void *instruccion = leer_string_con_longitud(buffer, &desplazamiento); //esta bien usar string con long?
    uint32_t program_counter = leer_uint32(buffer, &desplazamiento);
    uint32_t estimacion_rafaga = leer_uint32(buffer, &desplazamiento);

    t_cpu_recibirinstruccion_response *response = recibirinstruccion_response_new(id, tamanio, instruccion, program_counter, estimacion_rafaga);

    return response;
}

int bytes_totales_recibirinstruccion_response_serializada(t_cpu_recibirinstruccion_response *response)
{
    // ID (uint32), TAMANIO (uint32), INSTRUCCION (void), PROGRAM_COUNTER (uint32), ESTIMACION_RAFAGA (uint32)
    int bytes = 0;

    bytes += sizeof(response->id);
    bytes += sizeof(response->tamanio);
    bytes += sizeof(response->instruccion);
    bytes += sizeof(response->program_counter);
    bytes += sizeof(response->estimacion_rafaga);

    return bytes;
}
