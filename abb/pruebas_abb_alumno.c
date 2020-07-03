#include "testing.h"
#include "abb.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LARGO 6

void pruebas_abb_vacio(void);
void pruebas_abb_simples(void);
void pruebas_abb_destruir_no_NULL(void);
void pruebas_abb_reemplazar(void);
void pruebas_abb_reemplazar_con_destruir(void);
void pruebas_abb_volumen(void);
void pruebas_abb_iter_externo(void);
void pruebas_abb_iter_volumen(void);
void pruebas_abb_iter_interno(void);

void pruebas_abb_alumno(void){
	pruebas_abb_vacio();
	pruebas_abb_simples();
	pruebas_abb_destruir_no_NULL();
	pruebas_abb_reemplazar();
	pruebas_abb_reemplazar_con_destruir();
	//pruebas_abb_volumen(void);
	//pruebas_abb_iter_externo();
	//pruebas_abb_iter_volumen();
	//pruebas_abb_iter_interno();
}

void pruebas_abb_vacio(void){
	abb_t* abb = abb_crear(strcmp, NULL);
	char* claves[] = {"gonzalo", "adrian"};
	print_test("Prueba borrar abb vacio", abb_borrar(abb, claves[0]) == NULL);
	print_test("Prueba pertenece abb vacio", abb_pertenece(abb, claves[1]) == false);
	print_test("Prueba obtener abb vacio", abb_obtener(abb, claves[0]) == NULL);
	print_test("Prueba cantidad abb vacio", abb_cantidad(abb) == 0);
	abb_destruir(abb);
}

void pruebas_abb_simples(void){
	abb_t* abb = abb_crear(strcmp, NULL);
	char* claves[LARGO] = {"gonzalo",
						"adrian",
						"perro",
						"gato",
						"computadora",
						"teclado"};

	int datos[LARGO] = {1000, 100, 50, 36, 41, 22};

	print_test("Prueba guardar clave gonzalo", abb_guardar(abb, claves[0], &datos[0]));
	print_test("Prueba clave gonzalo pertenece", abb_pertenece(abb, claves[0]));
	print_test("Prueba clave gonzalo obtener", abb_obtener(abb, claves[0]) == &datos[0]);
	print_test("Cantidad aumento", abb_cantidad(abb) == 1);

	print_test("Prueba guardar clave adrian", abb_guardar(abb, claves[1], &datos[1]));
	print_test("Prueba clave adrian pertenece", abb_pertenece(abb, claves[1]));
	print_test("Prueba clave adrian obtener", abb_obtener(abb, claves[1]) == &datos[1]);
	print_test("Cantidad aumento", abb_cantidad(abb) == 2);

	print_test("Prueba guardar clave perro", abb_guardar(abb, claves[2], &datos[2]));
	print_test("Prueba clave perro pertenece", abb_pertenece(abb, claves[2]));
	print_test("Prueba clave perro obtener", abb_obtener(abb, claves[2]) == &datos[2]);
	print_test("Cantidad aumento", abb_cantidad(abb) == 3);

	print_test("Prueba borrar clave adrian (1 hijo)", abb_borrar(abb, claves[1]) == &datos[1]);
	print_test("Actualizo cantidad", abb_cantidad(abb) == 2);
	print_test("Clave adrian pertenece, es false", !abb_pertenece(abb, claves[1]));

	print_test("Prueba guardar clave gato", abb_guardar(abb, claves[3], &datos[3]));
	print_test("Prueba clave gato pertenece", abb_pertenece(abb, claves[3]));
	print_test("Prueba clave gato obtener", abb_obtener(abb, claves[3]) == &datos[3]);
	print_test("Cantidad aumento", abb_cantidad(abb) == 3);

	print_test("Prueba borrar clave gato (1 hijo)", abb_borrar(abb, claves[3]) == &datos[3]);
	print_test("Actualizo cantidad", abb_cantidad(abb) == 2);
	print_test("Clave gato pertenece, es false", !abb_pertenece(abb, claves[3]));

	print_test("Prueba guardar clave computadora", abb_guardar(abb, claves[4], &datos[4]));
	print_test("Prueba clave computadora pertenece", abb_pertenece(abb, claves[4]));
	print_test("Prueba clave computadora obtener", abb_obtener(abb, claves[4]) == &datos[4]);
	print_test("Cantidad aumento", abb_cantidad(abb) == 3);

	print_test("Prueba guardar clave teclado", abb_guardar(abb, claves[5], &datos[5]));
	print_test("Prueba clave teclado pertenece", abb_pertenece(abb, claves[5]));
	print_test("Prueba clave teclado obtener", abb_obtener(abb, claves[5]) == &datos[5]);
	print_test("Cantidad aumento", abb_cantidad(abb) == 4);

	print_test("Prueba borrar clave teclado (0 hijos)", abb_borrar(abb, claves[5]) == &datos[5]);
	print_test("Actualizo cantidad", abb_cantidad(abb) == 3);
	print_test("Clave teclado pertenece, es false", !abb_pertenece(abb, claves[5]));

	print_test("Prueba borrar clave gonzalo (2 hijos)", abb_borrar(abb, claves[0]) == &datos[0]);
	print_test("Actualizo cantidad", abb_cantidad(abb) == 2);
	print_test("Clave gonzalo pertenece, es false", !abb_pertenece(abb, claves[0]));

	print_test("Prueba borrar clave computadora (0 hijos)", abb_borrar(abb, claves[4]) == &datos[4]);
	print_test("Actualizo cantidad", abb_cantidad(abb) == 1);
	print_test("Clave computadora pertenece, es false", !abb_pertenece(abb, claves[4]));

	print_test("Prueba borrar clave perro (0 hijos)", abb_borrar(abb, claves[2]) == &datos[2]);
	print_test("Actualizo cantidad", abb_cantidad(abb) == 0);
	print_test("Clave perro pertenece, es false", !abb_pertenece(abb, claves[2]));

	abb_destruir(abb);
}

void pruebas_abb_destruir_no_NULL(void){
	abb_t* abb = abb_crear(strcmp, free);
	char* claves[2] = {"hola",
					   "chau"};

	int *valor1, *valor2;
	valor1 = malloc(sizeof(int));
	valor2 = malloc(sizeof(int));

	print_test("Guardo primer par", abb_guardar(abb, claves[0], valor1));
	print_test("Guardo segundo par", abb_guardar(abb, claves[1], valor2));

	print_test("Primera clave pertenece", abb_pertenece(abb, claves[0]));
	print_test("Segunda clave pertenece", abb_pertenece(abb, claves[1]));

	print_test("Cantidad es 2", abb_cantidad(abb) == 2);

	abb_destruir(abb);
}

void pruebas_abb_reemplazar(void){
    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave1 = "perro", *valor1a = "guau", *valor1b = "warf";
    char *clave2 = "gato", *valor2a = "miau", *valor2b = "meaow";

    /* Inserta 2 valores y luego los reemplaza */
    print_test("Prueba guardar clave1", abb_guardar(abb, clave1, valor1a));
    print_test("Prueba obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba guardar clave2", abb_guardar(abb, clave2, valor2a));
    print_test("Prueba obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba guardar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("Prueba obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba guarda clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
    print_test("Prueba obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    abb_destruir(abb);
}

void pruebas_abb_reemplazar_con_destruir(void){
    abb_t* abb = abb_crear(strcmp, free);

    char *clave1 = "perro", *valor1a, *valor1b;
    char *clave2 = "gato", *valor2a, *valor2b;

    /* Pide memoria para 4 valores */
    valor1a = malloc(10 * sizeof(char));
    valor1b = malloc(10 * sizeof(char));
    valor2a = malloc(10 * sizeof(char));
    valor2b = malloc(10 * sizeof(char));

    /* Inserta 2 valores y luego los reemplaza (debe liberar lo que reemplaza) */
    print_test("Prueba guardar clave1", abb_guardar(abb, clave1, valor1a));
    print_test("Prueba obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba guardar clave2", abb_guardar(abb, clave2, valor2a));
    print_test("Prueba obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba guardar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("Prueba obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba guardar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
    print_test("Prueba obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    /* Se destruye el abb (se debe liberar lo que quedó dentro) */
    abb_destruir(abb);
}

