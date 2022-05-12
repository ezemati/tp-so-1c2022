#include <cpu/obtener_dato.h>

//Crea struct para enviar dir fisica
t_cpu_obtenerdato_request *obtenerdato_request_new(uint32_t direccion_fisica, uint32_t cantidad_bytes){
    t_cpu_obtenerdato_request *request = malloc(sizeof(t_cpu_obtenerdato_request));
    request->direccion_fisica = direccion_fisica;
    request->cantidad_bytes = cantidad_bytes;
    return request;
    }

void obtenerdato_request_destroy(t_cpu_obtenerdato_request *request){
    free(request);
}

//Crea struct para recibir el dato en la dir fisica
t_cpu_obtenerdato_response *obtenerdato_response_new(uint32_t cantidad_bytes, void *dato){
    t_cpu_obtenerdato_response *response = malloc(sizeof(t_cpu_obtenerdato_response));
    request->cantidad_bytes = cantidad_bytes;
    response->dato = malloc(response->cantidad_bytes);
    memcpy(response->dato, *dato, response->cantidad_bytes);
    return response;
}
void obtenerdato_response_destroy(t_cpu_obtenerdato_response *response){
    free(response);
}