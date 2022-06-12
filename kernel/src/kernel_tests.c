#include <kernel_tests.h>

int run_tests()
{
    CU_initialize_registry();
    CU_pSuite tests = CU_add_suite("KERNEL Suite", NULL, NULL);

    CU_add_test(tests, "Serializado/Deserializado actualizar_pcb", serializar_deserializar_actualizarpcb_request_response);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

void serializar_deserializar_actualizarpcb_request_response()
{
    double time_inicio = time(NULL);
    double time_fin = time(NULL);

    t_kernel_actualizarpcb_request *request = actualizarpcb_request_new(1, 2, 3, time_inicio, time_fin);
    int bytes_serializado_request;
    void *request_serializada = serializar_actualizarpcb_request(request, &bytes_serializado_request);
    t_kernel_actualizarpcb_request *request_deserializada = deserializar_actualizarpcb_request(request_serializada);
    CU_ASSERT_EQUAL(request_deserializada->pid, 1);
    CU_ASSERT_EQUAL(request_deserializada->program_counter, 2);
    CU_ASSERT_EQUAL(request_deserializada->bloqueo_pendiente, 3);
    CU_ASSERT_EQUAL(0, memcmp(&(request_deserializada->time_inicio_running), &time_inicio, sizeof(double)));
    CU_ASSERT_EQUAL(0, memcmp(&(request_deserializada->time_fin_running), &time_fin, sizeof(double)));
    actualizarpcb_request_destroy(request_deserializada);
    free(request_serializada);
    actualizarpcb_request_destroy(request);

    // t_memoria_escribirdato_response *response = escribirdato_response_new(1);
    // int bytes_serializado_response;
    // void *response_serializada = serializar_escribirdato_response(response, &bytes_serializado_response);
    // t_memoria_escribirdato_response *response_deserializada = deserializar_escribirdato_response(response_serializada);
    // CU_ASSERT_EQUAL(response_deserializada->cantidad_bytes, 1);
    // escribirdato_response_destroy(response_deserializada);
    // free(response_serializada);
    // escribirdato_response_destroy(response);
}
