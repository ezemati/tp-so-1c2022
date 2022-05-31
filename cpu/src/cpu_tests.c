#include <cpu_tests.h>

static void assert_instruccion_serializada(t_instruccion *instruccion_serializada, t_instruccion *instruccion);

int run_tests()
{
    CU_initialize_registry();
    CU_pSuite tests = CU_add_suite("CPU Suite", NULL, NULL);

    CU_add_test(tests, "Serializado/Deserializado ejecutar_proceso", serializar_deserializar_ejecutarproceso_request_response);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

void serializar_deserializar_ejecutarproceso_request_response()
{
    t_list *lista_instrucciones = list_create();

    t_instruccion *instruccion0 = instruccion_new_with_numeric_params(NO_OP, 0, NULL);

    uint32_t parametros1[2] = {13, 26};
    t_instruccion *instruccion1 = instruccion_new_with_numeric_params(WRITE, 2, parametros1);

    list_add(lista_instrucciones, instruccion0);
    list_add(lista_instrucciones, instruccion1);

    t_cpu_ejecutarproceso_request *request = ejecutarproceso_request_new(1, 2, 3, 4, lista_instrucciones);
    int bytes_serializado_request;
    void *request_serializada = serializar_ejecutarproceso_request(request, &bytes_serializado_request);
    t_cpu_ejecutarproceso_request *request_deserializada = deserializar_ejecutarproceso_request(request_serializada);
    CU_ASSERT_EQUAL(request_deserializada->pid, request->pid);
    CU_ASSERT_EQUAL(request_deserializada->tamanio, request->tamanio);
    CU_ASSERT_EQUAL(request_deserializada->program_counter, request->program_counter);
    CU_ASSERT_EQUAL(request_deserializada->tabla_paginas_primer_nivel, request->tabla_paginas_primer_nivel);
    assert_instruccion_serializada(list_get(request_deserializada->lista_instrucciones, 0), instruccion0);
    assert_instruccion_serializada(list_get(request_deserializada->lista_instrucciones, 1), instruccion1);
    ejecutarproceso_request_destroy(request_deserializada);
    free(request_serializada);
    ejecutarproceso_request_destroy(request);

    t_cpu_ejecutarproceso_response *response = ejecutarproceso_response_new(true);
    int bytes_serializado_response;
    void *response_serializada = serializar_ejecutarproceso_response(response, &bytes_serializado_response);
    t_cpu_ejecutarproceso_response *response_deserializada = deserializar_ejecutarproceso_response(response_serializada);
    CU_ASSERT_EQUAL(response_deserializada->ok, true);
    ejecutarproceso_response_destroy(response_deserializada);
    free(response_serializada);
    ejecutarproceso_response_destroy(response);

    instrucciones_destroy(lista_instrucciones);
}

static void assert_instruccion_serializada(t_instruccion *instruccion_serializada, t_instruccion *instruccion)
{
    CU_ASSERT_EQUAL(instruccion_serializada->codigo_instruccion, instruccion->codigo_instruccion);

    CU_ASSERT_EQUAL(instruccion_serializada->cantidad_parametros, instruccion->cantidad_parametros);

    for (int i = 0; i < instruccion_serializada->cantidad_parametros; i++)
    {
        CU_ASSERT_EQUAL(instruccion_serializada->parametros[i], instruccion->parametros[i]);
    }
}
