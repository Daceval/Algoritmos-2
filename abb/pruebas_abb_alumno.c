#include "testing.h"
#include "abb.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

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
	pruebas_abb_volumen();
	pruebas_abb_iter_volumen();
	pruebas_abb_iter_externo();
	pruebas_abb_iter_interno();
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


void pruebas_abb_volumen(void){

	size_t largo = 1000;
	abb_t* abb = abb_crear(strcmp, NULL);
	
	const size_t largo_clave = 20;
	char (*claves)[largo_clave] = malloc(largo * largo_clave);
	size_t *valores[largo];
	
	/* Inserta 'largo' parejas en el abb */
	bool ok = true;
	for(size_t i = 0; i < largo; i++){
		valores[i] = malloc(sizeof(size_t));
		sprintf(claves[i], "%08ld", i);
		*valores[i] = i;
		ok = abb_guardar(abb, claves[i], valores[i]);
		if(!ok){
			printf("NO GUARDO TODO\n");
			break;
		}
	}
	print_test("prueba abb se agregaron muchos elementos ", ok);
	print_test("prueba abb cantidad de elementos es la correcta", abb_cantidad(abb) == largo);

	 /* Verifica que devuelva los valores correctos */
	for (size_t i = 0; i < largo; i++) {
		ok = abb_pertenece(abb, claves[i]);
  	 	if (!ok) break;
  	 	ok = abb_obtener(abb, claves[i]) == valores[i];
  	 	if (!ok) break;
    }

    print_test("Prueba abb pertenece y obtener muchos elementos", ok);
    print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb) == largo);

    /* Verifica que borre y devuelva los valores correctos */
    for (size_t i = 0; i < largo; i++){
    	ok = abb_borrar(abb, claves[i]) == valores[i];
    	if(!ok){
    		break;
    	}
    }
    print_test("Prueba abb borrar muchos elementos", ok);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    /* Destruye el abb*/
  	abb_destruir(abb);
	
  	abb = abb_crear(strcmp, free);
	ok = true;
    for (size_t i = 0; i < largo; i++) {
        ok = abb_guardar(abb, claves[i], valores[i]);
        if (!ok) break;
    }
 
    free(claves);

    /* Destruye el abb - debería liberar los enteros */
    abb_destruir(abb);
} 


void pruebas_abb_iter_volumen(){

	size_t largo = 1500;
	abb_t* abb = abb_crear(strcmp, NULL);
	
	size_t largo_clave = 20;
	char (*claves)[largo_clave] = malloc(largo * largo_clave);
	size_t valores[largo];
	
	/* Inserta 'largo' parejas en el abb */
	bool ok = true;
	for(size_t i = 0; i < largo; i++){
		sprintf(claves[i], "%zu", i);
		valores[i] = i;
		ok = abb_guardar(abb, claves[i], &valores[i]);
		if(!ok){
			break;
		}
	}

	/* Pruebas de iteracion */
	abb_iter_t* iter = abb_iter_in_crear(abb);
	print_test("prueba del abb iterador esta al final es false", !abb_iter_in_al_final(iter));

	ok = true;
	const char* clave;
	size_t* valor;
	size_t i = 0;
	for(i = 0 ; i < largo; i++){
		if (abb_iter_in_al_final(iter)){
			ok = false;
			break;
		}
		clave = abb_iter_in_ver_actual(iter);
		if(clave == NULL){
			ok = false;
			break;
		}
		valor = abb_obtener(abb, clave);
		if (valor == NULL){
			ok = false;
			break;
		}
		*valor = largo;
		abb_iter_in_avanzar(iter);
	}
	print_test("Prueba abb iteración en volumen", ok);
    print_test("Prueba abb iteración en volumen, recorrio todo el largo", i == largo);
    print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

    ok = true;
    for (i = 0; i < largo; i++) {
        if ( valores[i] != largo ) {
            ok = false;
            break;
        }
    }
    print_test("prueba abb iteración en volumen, se cambiaron todo los elementos", ok);
    free(claves);
    
    /* Destruyo iterador*/
    abb_iter_in_destruir(iter);
    
    /* Destruyo abb */
    abb_destruir(abb);
}


static ssize_t buscar(const char* clave, char* claves[], size_t largo)
{
    for (size_t i = 0; i < largo; i++) {
        if (strcmp(clave, claves[i]) == 0) return (ssize_t) i;
    }
    return -1;
}

void pruebas_abb_iter_externo(){
    
    abb_t *abb = abb_crear(strcmp, NULL);

    char *claves[] = {"perro", "gato", "vaca"};
    char *valores[] = {"guau", "miau", "mu"};

    /* Inserta 3 valores */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, claves[0], valores[0]));
    print_test("Prueba abb insertar clave2", abb_guardar(abb, claves[1], valores[1]));
    print_test("Prueba abb insertar clave3", abb_guardar(abb, claves[2], valores[2]));

    // Prueba de iteración sobre las claves almacenadas.
    abb_iter_t *iter = abb_iter_in_crear(abb);
    const char *clave;
    ssize_t indice;

    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    /* Primer valor */
    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    print_test("Prueba abb iterador avanzar es true", abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    /* Segundo valor */
    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    print_test("Prueba abb iterador avanzar es true", abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    /* Tercer valor */
    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    abb_iter_in_avanzar(iter);
    print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

    /* Vuelve a tratar de avanzar, por las dudas */
    print_test("Prueba abb iterador ver actual, es NULL", !abb_iter_in_ver_actual(iter));
    print_test("Prueba abb iterador avanzar es false", !abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}



bool filtrar_claves_pares(const char* clave, void* valor, void* extra){
	size_t* res = extra;
	int cadena_a_numero = atoi(clave);
	if(cadena_a_numero % 2 == 0){
		*res += 1;
	}
	return true;
}

void pruebas_abb_iter_interno(void){
	abb_t* abb = abb_crear(strcmp, NULL);
	char* claves[] = {"2", "3", "4", "7"};
	char* valores[] = {"2", "4", "6", "8"};

	/* Inserto valores en el abb */
	print_test("inserto clave1 en abb", abb_guardar(abb, claves[0], valores[0]));
	print_test("inserto clave2 en abb", abb_guardar(abb, claves[1], valores[1]));
	print_test("inserto clave3 en abb", abb_guardar(abb, claves[2], valores[2]));
	print_test("inserto clave4 en abb", abb_guardar(abb, claves[3], valores[3]));

	size_t resultado = 0;
	abb_in_order(abb, filtrar_claves_pares, &resultado);

	print_test("El iterador interno funciona correctamente", resultado == 2);
	/* Destruyo abb */
	abb_destruir(abb);
}