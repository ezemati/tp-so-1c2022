#include <consola_tests.h>

int run_tests()
{
    CU_initialize_registry();
    CU_pSuite tests = CU_add_suite("CONSOLA Suite", NULL, NULL);

    CU_add_test(tests, "Serializado/Deserializado", serializar_deserializar_instrucciones);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

void serializar_deserializar_instrucciones()
{
    t_list *instrucciones = list_create();

    t_instruccion *instruccion0 = instruccion_new_with_numeric_params(NO_OP, 0, NULL);

    uint32_t parametros1[2] = {13, 26};
    t_instruccion *instruccion1 = instruccion_new_with_numeric_params(WRITE, 2, parametros1);

    list_add(instrucciones, instruccion0);
    list_add(instrucciones, instruccion1);

    uint32_t tamanio_programa = 50;
    t_programa *programa = programa_new_with_instructions(instrucciones, tamanio_programa);

    int bytes_programa_serializado;
    void *serializado = serializar_programa(programa, &bytes_programa_serializado);

    int desplazamiento = 0;

    t_programa *programa_deserializado = deserializar_programa(serializado);

    CU_ASSERT_EQUAL(programa_deserializado->tamanio, tamanio_programa);

    CU_ASSERT_EQUAL(programa_cantidad_instrucciones(programa_deserializado), 2);

    assert_instruccion_serializada(list_get(programa_deserializado->instrucciones, 0), instruccion0);
    assert_instruccion_serializada(list_get(programa_deserializado->instrucciones, 1), instruccion1);

    programa_destroy(programa_deserializado);
    free(serializado);
    programa_destroy(programa);
}

void assert_instruccion_serializada(t_instruccion *instruccion_serializada, t_instruccion *instruccion)
{
    CU_ASSERT_EQUAL(instruccion_serializada->codigo_instruccion, instruccion->codigo_instruccion);

    CU_ASSERT_EQUAL(instruccion_serializada->cantidad_parametros, instruccion->cantidad_parametros);

    for (int i = 0; i < instruccion_serializada->cantidad_parametros; i++)
    {
        CU_ASSERT_EQUAL(instruccion_serializada->parametros[i], instruccion->parametros[i]);
    }
}
