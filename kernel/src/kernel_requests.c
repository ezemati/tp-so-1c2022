#include <kernel_requests.h>

void crear_proceso(int socket_cliente)
{
	uint32_t bytes_request;
	recibir_uint32_por_socket(socket_cliente, &bytes_request);

	void *buffer_request = malloc(bytes_request);
	recibir_por_socket(socket_cliente, buffer_request, bytes_request);

	t_programa *programa = deserializar_programa(buffer_request);

	log_debug(logger, "Recibi proceso con tamanio %d y %d instrucciones", programa->tamanio, programa_cantidad_instrucciones(programa));
	uint32_t proximo_pid = obtener_proximo_pid();
	t_kernel_pcb *nuevo_pcb = pcb_new(proximo_pid, programa);

	// TODO: hacer request a memoria para obtener la tabla de primer nivel de este nuevo proceso
	list_add(lista_procesos, nuevo_pcb);

	uint32_t response = 1;
	enviar_uint32_por_socket(socket_cliente, response);

	programa_destroy(programa);
	free(buffer_request);
}
