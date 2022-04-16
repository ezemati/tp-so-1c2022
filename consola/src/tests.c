#include "tests.h"
#include "utils.h"

#include <stdlib.h>
#include <stdint.h>
#include <CUnit/Basic.h>

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

    t_instruccion *instruccion1 = malloc(sizeof(t_instruccion));
    instruccion1->codigo_instruccion = strdup("NO_OP");
    instruccion1->cantidad_parametros = 0;

    t_instruccion *instruccion2 = malloc(sizeof(t_instruccion));
    instruccion2->codigo_instruccion = strdup("WRITE");
    instruccion2->cantidad_parametros = 2;
    instruccion2->parametros[0] = 13;
    instruccion2->parametros[1] = 26;

    list_add(instrucciones, instruccion1);
    list_add(instrucciones, instruccion2);

    uint32_t tamanio_programa = 50;

    int bytes;
    void *serializado = serializar_instrucciones(instrucciones, tamanio_programa, &bytes);

    int desplazamiento = 0;

    uint32_t tamanio_deserializado;
    memcpy(&tamanio_deserializado, serializado + desplazamiento, sizeof(uint32_t));
    CU_ASSERT_EQUAL(tamanio_deserializado, tamanio_programa);
    desplazamiento += sizeof(uint32_t);

    assert_instruccion_serializada(serializado, instruccion1, &desplazamiento);
    assert_instruccion_serializada(serializado, instruccion2, &desplazamiento);
}

void assert_instruccion_serializada(void *serializado, t_instruccion *instruccion, int *desplazamiento)
{
    uint32_t tamanio_codigo_deserializado;
    memcpy(&tamanio_codigo_deserializado, serializado + (*desplazamiento), sizeof(uint32_t));
    CU_ASSERT_EQUAL(tamanio_codigo_deserializado, strlen(instruccion->codigo_instruccion) + 1);
    (*desplazamiento) += sizeof(uint32_t);

    char *codigo1_deserializado = malloc(tamanio_codigo_deserializado);
    memcpy(&codigo1_deserializado, serializado + (*desplazamiento), tamanio_codigo_deserializado);
    CU_ASSERT_STRING_EQUAL(codigo1_deserializado, instruccion->codigo_instruccion);
    (*desplazamiento) += tamanio_codigo_deserializado;

    uint32_t cant_parametros_deserializado;
    memcpy(&cant_parametros_deserializado, serializado + (*desplazamiento), sizeof(uint32_t));
    CU_ASSERT_EQUAL(cant_parametros_deserializado, instruccion->cantidad_parametros);
    (*desplazamiento) += sizeof(uint32_t);

    for (int i = 0; i < cant_parametros_deserializado; i++)
    {
        uint32_t parametro_deserializado;
        memcpy(&parametro_deserializado, serializado + (*desplazamiento), sizeof(uint32_t));
        CU_ASSERT_EQUAL(parametro_deserializado, instruccion->parametros[i]);
        (*desplazamiento) += sizeof(uint32_t);
    }
}
