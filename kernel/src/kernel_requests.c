#include <kernel_requests.h>

void crear_proceso(int socket_cliente)
{
	void *buffer_request = NULL;
	recibir_buffer_con_bytes_por_socket(socket_cliente, &buffer_request);

	t_programa *programa = deserializar_programa(buffer_request);

	log_debug(logger, "Recibi proceso con tamanio %d y %d instrucciones", programa->tamanio, programa_cantidad_instrucciones(programa));
	uint32_t proximo_pid = obtener_proximo_pid();
	t_kernel_pcb *nuevo_pcb = pcb_new(proximo_pid, socket_cliente, programa);

	list_add(lista_procesos, nuevo_pcb);
	list_add(lista_new, nuevo_pcb);
	intentar_pasar_proceso_a_memoria();

	// print_instrucciones_de_todos_los_procesos(lista_procesos); // Para Debug

	programa_destroy(programa);
	free(buffer_request);
}
