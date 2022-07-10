#include <kernel_requests.h>

void atender_crear_proceso(int socket_cliente)
{
	void *buffer_request = NULL;
	recibir_buffer_con_bytes_por_socket(socket_cliente, &buffer_request);

	t_programa *programa = deserializar_programa(buffer_request);

	log_info_with_mutex(logger, &mutex_logger, "Recibi proceso con tamanio %d y %d instrucciones", programa->tamanio, programa_cantidad_instrucciones(programa));
	uint32_t pid = obtener_proximo_pid();
	t_kernel_pcb *nuevo_pcb = pcb_new(pid, socket_cliente, programa);
	log_info_with_mutex(logger, &mutex_logger, "Proceso %d en NEW", nuevo_pcb->id);

	list_add_with_mutex(lista_procesos, nuevo_pcb, &mutex_lista_procesos);

	list_add_with_mutex(lista_new, nuevo_pcb, &mutex_lista_new);

	intentar_pasar_proceso_a_memoria();

	// print_instrucciones_de_todos_los_procesos(lista_procesos); // Para Debug

	programa_destroy(programa);
	free(buffer_request);
}

void atender_bloquear_proceso(int socket_cliente)
{
	pthread_mutex_lock(&mutex_hay_proceso_en_ejecucion);
	hay_proceso_en_ejecucion = false;
	pthread_mutex_unlock(&mutex_hay_proceso_en_ejecucion);

	void *buffer_request = NULL;
	recibir_buffer_con_bytes_por_socket(socket_cliente, &buffer_request);

	t_kernel_actualizarpcb_request *request = deserializar_actualizarpcb_request(buffer_request);

	t_kernel_pcb *pcb = obtener_proceso_por_pid(request->pid);
	actualizar_pcb_bloqueado(pcb, request->program_counter, request->time_inicio_running, request->time_fin_running);

	bloquear_proceso(pcb, request->bloqueo_pendiente);

	actualizarpcb_request_destroy(request);
	free(buffer_request);
}

void atender_finalizar_proceso(int socket_cliente)
{
	pthread_mutex_lock(&mutex_hay_proceso_en_ejecucion);
	hay_proceso_en_ejecucion = false;
	pthread_mutex_unlock(&mutex_hay_proceso_en_ejecucion);

	void *buffer_request = NULL;
	recibir_buffer_con_bytes_por_socket(socket_cliente, &buffer_request);

	t_kernel_actualizarpcb_request *request = deserializar_actualizarpcb_request(buffer_request);

	t_kernel_pcb *pcb = obtener_proceso_por_pid(request->pid);
	actualizar_pcb_bloqueado(pcb, request->program_counter, request->time_inicio_running, request->time_fin_running); // Innecesario, pero por las dudas

	finalizar_proceso(pcb);

	actualizarpcb_request_destroy(request);
	free(buffer_request);
}
