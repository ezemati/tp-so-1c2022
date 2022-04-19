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

    t_instruccion *instruccion1 = instruccion_new_with_numeric_params("NO_OP", 0, NULL);

    uint32_t parametros2[2] = {13, 26};
    t_instruccion *instruccion2 = instruccion_new_with_numeric_params("WRITE", 2, parametros2);

    list_add(instrucciones, instruccion1);
    list_add(instrucciones, instruccion2);

    uint32_t tamanio_programa = 50;
    t_programa *programa = programa_new_with_instructions(instrucciones, tamanio_programa);

    int bytes_programa_serializado;
    void *serializado = serializar_programa(programa, &bytes_programa_serializado);

    int desplazamiento = 0;

    uint32_t bytes_programa_deserializado = leer_uint32(serializado, &desplazamiento);
    CU_ASSERT_EQUAL(bytes_programa_deserializado, bytes_programa_serializado - sizeof(uint32_t)); // El deserializado no tiene en cuenta el uint32 inicial con el tamanio total

    uint32_t tamanio_deserializado = leer_uint32(serializado, &desplazamiento);
    CU_ASSERT_EQUAL(tamanio_deserializado, tamanio_programa);

    uint32_t cant_instrucciones_deserializado = leer_uint32(serializado, &desplazamiento);
    CU_ASSERT_EQUAL(cant_instrucciones_deserializado, programa_cantidad_instrucciones(programa));

    assert_instruccion_serializada(serializado, instruccion1, &desplazamiento);
    assert_instruccion_serializada(serializado, instruccion2, &desplazamiento);
}

void assert_instruccion_serializada(void *serializado, t_instruccion *instruccion, int *desplazamiento)
{
    uint32_t bytes_instruccion_deserializada = leer_uint32(serializado, desplazamiento);

    uint32_t tamanio_codigo_deserializado = leer_uint32(serializado, desplazamiento);
    CU_ASSERT_EQUAL(tamanio_codigo_deserializado, strlen(instruccion->codigo_instruccion) + 1);

    char *codigo_deserializado = leer_string(serializado, tamanio_codigo_deserializado, desplazamiento);
    CU_ASSERT_STRING_EQUAL(codigo_deserializado, instruccion->codigo_instruccion);

    uint32_t cant_parametros_deserializado = leer_uint32(serializado, desplazamiento);
    CU_ASSERT_EQUAL(cant_parametros_deserializado, instruccion->cantidad_parametros);

    for (int i = 0; i < cant_parametros_deserializado; i++)
    {
        uint32_t parametro_deserializado = leer_uint32(serializado, desplazamiento);
        CU_ASSERT_EQUAL(parametro_deserializado, instruccion->parametros[i]);
    }
}
