#include <kernel_requests.h>

void request_proceso_consola_new(socket_cliente)
{
	uint32_t bytes_request;
	recibir_uint32_por_socket(socket_cliente, &bytes_request);

	void *buffer_request = malloc(bytes_request);
	recibir_por_socket(socket_cliente, buffer_request, bytes_request);

	t_kernel_request_proceso_consola *request = deserializar_request_proceso_consola(buffer_request);

	log_debug(logger, "Recibiendo tamaño de proceso %d y lista de instrucciones %s", request->tamanio_proceso, request->lista_instrucciones);
	t_kernel_pcb nuevo_pcb = crear_estructura_pcb(request->tamanio_proceso, request->lista_instrucciones);

	t_kernel_response_proceso_consola *response = response_proceso_consola(nuevo_pcb);
	int bytes;
	void *buffer_response = serializar_response_proceso_consola(response, &bytes);
	enviar_por_socket(socket_cliente, buffer_response, bytes);

	free(buffer_response);
	request_proceso_consola_destroy(request);
	response_proceso_consola_destroy(response);
	free(buffer_request);

}
