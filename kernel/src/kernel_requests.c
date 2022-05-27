#include <kernel_requests.h>

void atender_crear_proceso(int socket_cliente)
{
	void *buffer_request = NULL;
	recibir_buffer_con_bytes_por_socket(socket_cliente, &buffer_request);

	t_programa *programa = deserializar_programa(buffer_request);

	log_debug(logger, "Recibi proceso con tamanio %d y %d instrucciones", programa->tamanio, programa_cantidad_instrucciones(programa));
	uint32_t proximo_pid = obtener_proximo_pid();
	t_kernel_pcb *nuevo_pcb = pcb_new(proximo_pid, socket_cliente, programa);

	pthread_mutex_lock(&mutex_lista_procesos);
	list_add(lista_procesos, nuevo_pcb);
	pthread_mutex_unlock(&mutex_lista_procesos);

	pthread_mutex_lock(&mutex_lista_new);
	list_add(lista_new, nuevo_pcb);
	pthread_mutex_unlock(&mutex_lista_new);

	intentar_pasar_proceso_a_memoria();

	// print_instrucciones_de_todos_los_procesos(lista_procesos); // Para Debug

	programa_destroy(programa);
	free(buffer_request);
}

void atender_bloquear_proceso(int socket_cliente)
{
	void *buffer_request = NULL;
	recibir_buffer_con_bytes_por_socket(socket_cliente, &buffer_request);

	t_kernel_actualizarpcb_request *request = deserializar_actualizarpcb_request(buffer_request);

	t_kernel_pcb *pcb = obtener_proceso_por_pid(request->pid);
	pcb->program_counter = request->program_counter;
	cargar_tiempo_ejecucion_en_cpu(pcb, request->time_inicio_running, request->time_fin_running);
	recalcular_estimacion(pcb);

	bloquear_o_suspender_proceso(pcb, request->bloqueo_pendiente);

	actualizarpcb_request_destroy(request);
	free(buffer_request);
}

void atender_finalizar_proceso(int socket_cliente)
{
	void *buffer_request = NULL;
	recibir_buffer_con_bytes_por_socket(socket_cliente, &buffer_request);

	t_kernel_actualizarpcb_request *request = deserializar_actualizarpcb_request(buffer_request);

	t_kernel_pcb *pcb = obtener_proceso_por_pid(request->pid);
	pcb->program_counter = request->program_counter;
	cargar_tiempo_ejecucion_en_cpu(pcb, request->time_inicio_running, request->time_fin_running);
	recalcular_estimacion(pcb);

	finalizar_proceso(pcb);

	actualizarpcb_request_destroy(request);
	free(buffer_request);
}
