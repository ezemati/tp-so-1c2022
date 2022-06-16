#include <memoria_tests.h>

int run_tests()
{
    CU_initialize_registry();
    CU_pSuite tests = CU_add_suite("MEMORIA Suite", NULL, NULL);

    CU_add_test(tests, "Serializado/Deserializado escribir_dato", serializar_deserializar_escribirdato_request_response);
    CU_add_test(tests, "Serializado/Deserializado finalizar_proceso", serializar_deserializar_finalizarproceso_request_response);
    CU_add_test(tests, "Serializado/Deserializado handshake_configuraciones", serializar_deserializar_handshakeconfiguraciones_response);
    CU_add_test(tests, "Serializado/Deserializado inicializar_proceso", serializar_deserializar_inicializarproceso_request_response);
    CU_add_test(tests, "Serializado/Deserializado leer_dato", serializar_deserializar_leerdato_request_response);
    CU_add_test(tests, "Serializado/Deserializado marco_para_entrada_tabla_2", serializar_deserializar_marcoparaentradatabla2_request_response);
    CU_add_test(tests, "Serializado/Deserializado numero_tabla_2_para_entrada_tabla_1", serializar_deserializar_numerotabla2paraentradatabla1_request_response);
    CU_add_test(tests, "Serializado/Deserializado suspender_proceso", serializar_deserializar_suspenderproceso_request_response);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

void serializar_deserializar_escribirdato_request_response()
{
    char *dato = "12345";
    uint32_t bytes_dato = strlen(dato) + 1;
    t_memoria_escribirdato_request *request = escribirdato_request_new(1, 2, bytes_dato, dato);
    int bytes_serializado_request;
    void *request_serializada = serializar_escribirdato_request(request, &bytes_serializado_request);
    t_memoria_escribirdato_request *request_deserializada = deserializar_escribirdato_request(request_serializada);
    CU_ASSERT_EQUAL(request_deserializada->pid, 1);
    CU_ASSERT_EQUAL(request_deserializada->direccion_fisica, 2);
    CU_ASSERT_EQUAL(request_deserializada->cantidad_bytes, bytes_dato);
    CU_ASSERT_EQUAL(0, memcmp(request_deserializada->dato, dato, bytes_dato));
    escribirdato_request_destroy(request_deserializada);
    free(request_serializada);
    escribirdato_request_destroy(request);

    t_memoria_escribirdato_response *response = escribirdato_response_new(1);
    int bytes_serializado_response;
    void *response_serializada = serializar_escribirdato_response(response, &bytes_serializado_response);
    t_memoria_escribirdato_response *response_deserializada = deserializar_escribirdato_response(response_serializada);
    CU_ASSERT_EQUAL(response_deserializada->cantidad_bytes, 1);
    escribirdato_response_destroy(response_deserializada);
    free(response_serializada);
    escribirdato_response_destroy(response);
}

void serializar_deserializar_finalizarproceso_request_response()
{
    t_memoria_finalizarproceso_request *request = finalizarproceso_request_new(1, 2);
    int bytes_serializado_request;
    void *request_serializada = serializar_finalizarproceso_request(request, &bytes_serializado_request);
    t_memoria_finalizarproceso_request *request_deserializada = deserializar_finalizarproceso_request(request_serializada);
    CU_ASSERT_EQUAL(request_deserializada->pid, 1);
    CU_ASSERT_EQUAL(request_deserializada->numero_tablaprimernivel, 2);
    finalizarproceso_request_destroy(request_deserializada);
    free(request_serializada);
    finalizarproceso_request_destroy(request);

    /*
    t_memoria_finalizarproceso_response *response = finalizarproceso_response_new();
    int bytes_serializado_response;
    void *response_serializada = serializar_finalizarproceso_response(response, &bytes_serializado_response);
    t_memoria_finalizarproceso_response *response_deserializada = deserializar_finalizarproceso_response(response_serializada);
    finalizarproceso_response_destroy(response_deserializada);
    free(response_serializada);
    finalizarproceso_response_destroy(response);
    */
}

void serializar_deserializar_handshakeconfiguraciones_response()
{
    t_memoria_handshakeconfiguraciones_response *response = handshakeconfiguraciones_memoria_response_new(1, 2);
    int bytes_serializado_response;
    void *response_serializada = serializar_handshakeconfiguraciones_memoria_response(response, &bytes_serializado_response);
    t_memoria_handshakeconfiguraciones_response *response_deserializada = deserializar_handshakeconfiguraciones_memoria_response(response_serializada);
    CU_ASSERT_EQUAL(response_deserializada->entradas_por_tabla, 1);
    CU_ASSERT_EQUAL(response_deserializada->tamanio_pagina, 2);
    handshakeconfiguraciones_memoria_response_destroy(response_deserializada);
    free(response_serializada);
    handshakeconfiguraciones_memoria_response_destroy(response);
}

void serializar_deserializar_inicializarproceso_request_response()
{
    t_memoria_inicializarproceso_request *request = inicializarproceso_request_new(1, 2);
    int bytes_serializado_request;
    void *request_serializada = serializar_inicializarproceso_request(request, &bytes_serializado_request);
    t_memoria_inicializarproceso_request *request_deserializada = deserializar_inicializarproceso_request(request_serializada);
    CU_ASSERT_EQUAL(request_deserializada->pid, 1);
    CU_ASSERT_EQUAL(request_deserializada->tamanio_proceso, 2);
    inicializarproceso_request_destroy(request_deserializada);
    free(request_serializada);
    inicializarproceso_request_destroy(request);

    t_memoria_inicializarproceso_response *response = inicializarproceso_response_new(1);
    int bytes_serializado_response;
    void *response_serializada = serializar_inicializarproceso_response(response, &bytes_serializado_response);
    t_memoria_inicializarproceso_response *response_deserializada = deserializar_inicializarproceso_response(response_serializada);
    CU_ASSERT_EQUAL(response_deserializada->numero_tablaprimernivel, 1);
    inicializarproceso_response_destroy(response_deserializada);
    free(response_serializada);
    inicializarproceso_response_destroy(response);
}

void serializar_deserializar_leerdato_request_response()
{
    t_memoria_leerdato_request *request = leerdato_request_new(1, 2, 3);
    int bytes_serializado_request;
    void *request_serializada = serializar_leerdato_request(request, &bytes_serializado_request);
    t_memoria_leerdato_request *request_deserializada = deserializar_leerdato_request(request_serializada);
    CU_ASSERT_EQUAL(request_deserializada->pid, 1);
    CU_ASSERT_EQUAL(request_deserializada->direccion_fisica, 2);
    CU_ASSERT_EQUAL(request_deserializada->cantidad_bytes, 3);
    leerdato_request_destroy(request_deserializada);
    free(request_serializada);
    leerdato_request_destroy(request);

    char *dato_response = "12345";
    uint32_t bytes_dato_response = strlen(dato_response) + 1;
    t_memoria_leerdato_response *response = leerdato_response_new(dato_response, bytes_dato_response);
    int bytes_serializado_response;
    void *response_serializada = serializar_leerdato_response(response, &bytes_serializado_response);
    t_memoria_leerdato_response *response_deserializada = deserializar_leerdato_response(response_serializada);
    CU_ASSERT_EQUAL(response_deserializada->cantidad_bytes, bytes_dato_response);
    CU_ASSERT_EQUAL(0, memcmp(response_deserializada->dato, dato_response, bytes_dato_response));
    leerdato_response_destroy(response_deserializada);
    free(response_serializada);
    leerdato_response_destroy(response);
}

void serializar_deserializar_marcoparaentradatabla2_request_response()
{
    t_memoria_marcoparaentradatabla2_request *request = marcoparaentradatabla2_request_new(1, 2, 3);
    int bytes_serializado_request;
    void *request_serializada = serializar_marcoparaentradatabla2_request(request, &bytes_serializado_request);
    t_memoria_marcoparaentradatabla2_request *request_deserializada = deserializar_marcoparaentradatabla2_request(request_serializada);
    CU_ASSERT_EQUAL(request_deserializada->numero_tablaprimernivel, 1);
    CU_ASSERT_EQUAL(request_deserializada->numero_tablasegundonivel, 2);
    CU_ASSERT_EQUAL(request_deserializada->entrada_tablasegundonivel, 3);
    marcoparaentradatabla2_request_destroy(request_deserializada);
    free(request_serializada);
    marcoparaentradatabla2_request_destroy(request);

    t_memoria_marcoparaentradatabla2_response *response = marcoparaentradatabla2_response_new(1, -1);
    int bytes_serializado_response;
    void *response_serializada = serializar_marcoparaentradatabla2_response(response, &bytes_serializado_response);
    t_memoria_marcoparaentradatabla2_response *response_deserializada = deserializar_marcoparaentradatabla2_response(response_serializada);
    CU_ASSERT_EQUAL(response_deserializada->numero_marco, 1);
    CU_ASSERT_EQUAL(response_deserializada->numero_pagina_reemplazada, -1);
    marcoparaentradatabla2_response_destroy(response_deserializada);
    free(response_serializada);
    marcoparaentradatabla2_response_destroy(response);
}

void serializar_deserializar_numerotabla2paraentradatabla1_request_response()
{
    t_memoria_numerotabla2paraentradatabla1_request *request = numerotabla2paraentradatabla1_request_new(1, 2);
    int bytes_serializado_request;
    void *request_serializada = serializar_numerotabla2paraentradatabla1_request(request, &bytes_serializado_request);
    t_memoria_numerotabla2paraentradatabla1_request *request_deserializada = deserializar_numerotabla2paraentradatabla1_request(request_serializada);
    CU_ASSERT_EQUAL(request_deserializada->numero_tablaprimernivel, 1);
    CU_ASSERT_EQUAL(request_deserializada->entrada_tablaprimernivel, 2);
    numerotabla2paraentradatabla1_request_destroy(request_deserializada);
    free(request_serializada);
    numerotabla2paraentradatabla1_request_destroy(request);

    t_memoria_numerotabla2paraentradatabla1_response *response = numerotabla2paraentradatabla1_response_new(1);
    int bytes_serializado_response;
    void *response_serializada = serializar_numerotabla2paraentradatabla1_response(response, &bytes_serializado_response);
    t_memoria_numerotabla2paraentradatabla1_response *response_deserializada = deserializar_numerotabla2paraentradatabla1_response(response_serializada);
    CU_ASSERT_EQUAL(response_deserializada->numero_tablasegundonivel, 1);
    numerotabla2paraentradatabla1_response_destroy(response_deserializada);
    free(response_serializada);
    numerotabla2paraentradatabla1_response_destroy(response);
}

void serializar_deserializar_suspenderproceso_request_response()
{
    t_memoria_suspenderproceso_request *request = suspenderproceso_request_new(1, 2);
    int bytes_serializado_request;
    void *request_serializada = serializar_suspenderproceso_request(request, &bytes_serializado_request);
    t_memoria_suspenderproceso_request *request_deserializada = deserializar_suspenderproceso_request(request_serializada);
    CU_ASSERT_EQUAL(request_deserializada->pid, 1);
    CU_ASSERT_EQUAL(request_deserializada->numero_tablaprimernivel, 2);
    suspenderproceso_request_destroy(request_deserializada);
    free(request_serializada);
    suspenderproceso_request_destroy(request);

    t_memoria_suspenderproceso_response *response = suspenderproceso_response_new(false);
    int bytes_serializado_response;
    void *response_serializada = serializar_suspenderproceso_response(response, &bytes_serializado_response);
    t_memoria_suspenderproceso_response *response_deserializada = deserializar_suspenderproceso_response(response_serializada);
    CU_ASSERT_EQUAL(response_deserializada->ok, false);
    suspenderproceso_response_destroy(response_deserializada);
    free(response_serializada);
    suspenderproceso_response_destroy(response);

    t_memoria_suspenderproceso_response *response2 = suspenderproceso_response_new(true);
    int bytes_serializado_response2;
    void *response_serializada2 = serializar_suspenderproceso_response(response2, &bytes_serializado_response2);
    t_memoria_suspenderproceso_response *response_deserializada2 = deserializar_suspenderproceso_response(response_serializada2);
    CU_ASSERT_EQUAL(response_deserializada2->ok, true);
    suspenderproceso_response_destroy(response_deserializada2);
    free(response_serializada2);
    suspenderproceso_response_destroy(response2);
}
