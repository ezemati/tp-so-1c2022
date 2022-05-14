#include <kernel/request_proceso_consola.h>

t_kernel_request_proceso_consola *request_proceso_consola_new(uint32_t tamanio_proceso, char **lista_instrucciones)
{
	t_kernel_request_proceso_consola *request = malloc(sizeof(t_kernel_request_proceso_consola));
	request->tamanio_proceso = tamanio_proceso;
    request->lista_instrucciones = lista_instrucciones;
    return request;
}

void request_proceso_consola_destroy(t_kernel_request_proceso_consola *request)
{
    free(request);
}

t_kernel_response_proceso_consola *response_proceso_consola_new(t_kernel_pcb pcb)
{
	t_kernel_response_proceso_consola *response = malloc(sizeof(t_kernel_response_proceso_consola));
	response->pcb = pcb;
    return response;
}

void response_proceso_consola_destroy(t_kernel_response_proceso_consola *response)
{
    free(response);
}

/***************************** SERIALIZACION *****************************/

void *serializar_request_proceso_consola(t_kernel_request_proceso_consola *request, int *bytes)
{
	//El request recibe el tamaño del proceso y una lista de instrucciones
    uint32_t bytes_buffer_sin_tamanio = bytes_totales_request_proceso_consola_serializada(request);
    (*bytes) = sizeof(uint32_t) + bytes_buffer_sin_tamanio;
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    escribir_uint32(buffer, &desplazamiento, bytes_buffer_sin_tamanio);

    escribir_uint32(buffer, &desplazamiento, request->tamanio_proceso);
    escribir_uint32(buffer, &desplazamiento, request->lista_instrucciones);

    return buffer;
}

t_kernel_request_proceso_consola *deserializar_request_proceso_consola(void *buffer)
{
    int desplazamiento = 0;

    uint32_t tamanio_proceso = leer_uint32(buffer, &desplazamiento);
    char **lista_instrucciones = leer_lista_instrucciones(buffer, &desplazamiento);

    t_kernel_request_proceso_consola *request = request_proceso_consola(tamanio_proceso,lista_instrucciones);
    return request;
}

int bytes_totales_request_proceso_consola_serializada(t_kernel_request_proceso_consola *request)
{
    int bytes = 0;

    bytes += sizeof(request->tamanio_proceso);
    bytes += sizeof(request->lista_instrucciones);

    return bytes;
}

void *serializar_response_proceso_consola(t_kernel_response_proceso_consola *response, int *bytes)
{
    uint32_t bytes_buffer_sin_tamanio = bytes_totales_response_proceso_consola_serializada(response);
    (*bytes) = sizeof(uint32_t) + bytes_buffer_sin_tamanio;
    void *buffer = malloc(*bytes);

    int desplazamiento = 0;

    escribir_uint32(buffer, &desplazamiento, bytes_buffer_sin_tamanio);

    t_kernel_pcb pcb = response->pcb;

    escribir_uint32(buffer, &desplazamiento, pcb.id);
    escribir_uint32(buffer, &desplazamiento, pcb.tamanio);
    escribir_uint32(buffer, &desplazamiento, pcb.tamanio);
    escribir_string_con_longitud(buffer,&desplazamiento,pcb.program_counter);
    escribir_uint32(buffer, &desplazamiento, pcb.tabla_pagina);
    escribir_uint32(buffer, &desplazamiento, pcb.estimacion_rafaga);

    return buffer;
}

t_kernel_response_proceso_consola *deserializar_response_proceso_consola(void *buffer)
{
    int desplazamiento = 0;

    uint32_t id = leer_uint32(buffer, &desplazamiento);
    uint32_t tamanio_proceso = leer_uint32(buffer, &desplazamiento);
    char **lista_instrucciones; //TODO: leer del buffer y setearlo en el pcb
    char *program_counter = leer_string(buffer, sizeof(uint32_t), &desplazamiento); //revisar si es sizeof(uint32_t) el tamaño reservado
    uint32_t tabla_pagina = leer_uint32(buffer, &desplazamiento);
    uint32_t estimacion_rafaga = leer_uint32(buffer, &desplazamiento);

    t_kernel_pcb pcb = {id,tamanio_proceso,lista_instrucciones,program_counter,tabla_pagina,estimacion_rafaga};

    t_kernel_response_proceso_consola *response = response_proceso_consola(pcb);
    return response;
}

int bytes_totales_response_proceso_consola_serializada(t_kernel_response_proceso_consola *response)
{
    int bytes = 0;

    bytes += sizeof(response->pcb);

    return bytes;
}
