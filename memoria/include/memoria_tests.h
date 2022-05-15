#ifndef MEMORIA_TESTS_H
#define MEMORIA_TESTS_H

#include <memoria/escribir_dato.h>
#include <memoria/finalizar_proceso.h>
#include <memoria/handshake_configuraciones.h>
#include <memoria/inicializar_proceso.h>
#include <memoria/leer_dato.h>
#include <memoria/marco_para_entrada_tabla_2.h>
#include <memoria/numero_tabla_2_para_entrada_tabla_1.h>

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <CUnit/Basic.h>
#include <commons/collections/list.h>

int run_tests();
void serializar_deserializar_escribirdato_request_response();
void serializar_deserializar_finalizarproceso_request_response();
void serializar_deserializar_handshakeconfiguraciones_response();
void serializar_deserializar_inicializarproceso_request_response();
void serializar_deserializar_leerdato_request_response();
void serializar_deserializar_marcoparaentradatabla2_request_response();
void serializar_deserializar_numerotabla2paraentradatabla1_request_response();

#endif
