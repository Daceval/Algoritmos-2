#include <stddef.h>
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

#define LARGO 20

int cmp_func(const void* a, const void* b){
    int* n1 = (int*)a;
    int* n2 = (int*)b;
    if (*n1 < *n2)
        return -1;
    else if (*n1 > *n2)
        return 1;
    else
        return 0;
}

void prueba_heap_vacio(void){
    printf("\n----- Inicio de pruebas con heap vacio -----\n");

    heap_t*heap = heap_crear(cmp_func);
    print_test("Creando una heap distinto de NULL", heap != NULL);
    print_test("la accion de desencolar en un heap vacio es invalida", heap_desencolar(heap) == NULL);
    print_test("La accion de ver maximo en un heap es invalida", heap_ver_max(heap) == NULL);
    print_test("El heap esta vacio", heap_esta_vacio(heap) == true);
    print_test("la cantidad de elementos en el heap es 0", heap_cantidad(heap) == 0);

    /* Destruyo el heap */
    heap_destruir(heap, NULL);
    print_test("Se destruyo el heap correctamente", true);

}


void pruebas_heap_sort(void){
    printf("\n ---- Inicio de pruebas Heap Sort -----\n");

    heap_t* heap = heap_crear(cmp_func);

    void* elementos[LARGO];

    int arr1[LARGO] = {50, 40, 75, 20, 41, 65, 11, 1, -6, 8, 102, 1000, 6000, 25, 17, 15000, -3, -100, -2000, 50};
    int arr1_ord[LARGO] = {-2000, -100, -6, -3, 1, 8, 11, 17, 20, 25, 40, 41, 50, 50, 65, 75, 102, 1000, 6000, 15000};
    bool ok = true;
    for (size_t i = 0; i < LARGO; i++){
        elementos[i] = &arr1[i];
    }
    heap_sort(elementos, LARGO, cmp_func);
    for (size_t i = 0; i < LARGO; i++){
        if(*(int*)elementos[i] != arr1_ord[i]){
            printf("%d    %d    %ld\n",*(int*)elementos[i], arr1_ord[i],  i);
            ok = false;
            break;
        }
    }
    print_test("Prueba heap sort arreglo de 20 elementos", ok);
    heap_destruir(heap, NULL);
}

void pruebas_heap_destruir_no_NULL(void){
    printf("\n ----- Inicio de pruebas de heap destruir ----\n");
    heap_t* heap = heap_crear(cmp_func);
    int* valor1 = malloc(sizeof(int));
    int* valor2 = malloc(sizeof(int));
    *valor1 = 10000;
    *valor2 = 500;
    print_test("Prueba encolar un elemento", heap_encolar(heap, valor1));
    print_test("Prueba encolar segundo elemento", heap_encolar(heap, valor2));
    heap_destruir(heap, free);
}


void pruebas_varias_heap(void){
    printf("\n ----- Inicio de pruebas varias de heap ------\n");

    heap_t* heap = heap_crear(cmp_func);
    print_test("creando heap distinto de NULL", heap != NULL);
    print_test("El heap esta vacio", heap_esta_vacio(heap) == true);

    /* Creo variables para encolar en el heap */

    int elemento1 = 100;
    int elemento2 = 150;
    int elemento3 = 20;
    int elemento4 = 300;

    /* Encolo y desencolo elemento en el heap */

    print_test("Encolo el primer elemento entero(100) en el heap", heap_encolar(heap, &elemento1) == true);
    int* elemento_maximo = heap_ver_max(heap);
    print_test("el elemento maximo es el elemento que acabo de encolar", *elemento_maximo == elemento1);
    print_test("Encolo segundo elemento entero (150)", heap_encolar(heap, &elemento2) == true);
    print_test("la cantidad de elementos en el heap es 2", heap_cantidad(heap) == 2);

    print_test("el heap no esta vacio", heap_esta_vacio(heap) == false);
    print_test("el elemento maximo ahora es el entero (150)", *(int*)heap_ver_max(heap) == elemento2);

    print_test("Encolo el tercer elemento entero(20) en el heap", heap_encolar(heap, &elemento3) == true);
    print_test("Encolo un cuarto elemento entero(300) en el heap", heap_encolar(heap, &elemento4) == true);

    print_test("la cantidad de elementos en el heap es 4", heap_cantidad(heap) == 4);

    print_test("el elemento maximo ahora es el entero (300)", *(int*)heap_ver_max(heap) == elemento4);
    
    /* Desencolo y encolo elementos */
    
    int desencolado = *(int*)heap_desencolar(heap);
    print_test("desencolo un elemento que es el 300", desencolado == elemento4);
    desencolado = *(int*)heap_desencolar(heap);
    print_test("desencolo el elemento 150", desencolado == elemento2);

    print_test("vuelvo a encolar el ultimo elemento desencolado " ,heap_encolar(heap, &elemento2) == true);
    desencolado = *(int*)heap_desencolar(heap);
    print_test("desencolo de nuevo el elemento 150", desencolado == elemento2);
    print_test("El heap no esta vacio", heap_esta_vacio(heap) == false);
    print_test("el maximo es el elemento 100", *(int*)heap_ver_max(heap) == elemento1);
    desencolado = *(int*)heap_desencolar(heap);
    print_test("desencolo el elemento 100", desencolado == elemento1);
    desencolado = *(int*)heap_desencolar(heap);
    print_test("desencolo el ultimo elemento en el heap que es el 20", desencolado == elemento3);
    print_test("el heap vuelve a estar vacio", heap_esta_vacio(heap) == true);
    print_test("la accion de desencolar en un heap vacio es invalida", heap_desencolar(heap) == NULL);
    print_test("La accion de ver maximo en un heap es invalida", heap_ver_max(heap) == NULL);


    /* Destruyo el heap*/
    heap_destruir(heap, NULL);

    print_test("el heap se destruyo correctamente", true);
}

void heap_pruebas_volumen(size_t volumen){
    printf("\n -----Inicio de pruebas de volumen heap con (%zu) elementos ------\n", volumen);

    heap_t* heap = heap_crear(cmp_func);
    print_test("heap creado distinto de NULL", heap != NULL);

    bool encola_correctamente = true;
    bool maximos_correctos = true;

    /* Encolo elementos */
    
    for (unsigned i = 0; i < volumen; i++){
        int* elemento = malloc(sizeof(int));
        *elemento = i;
        bool encolado = heap_encolar(heap, elemento);
        int* maximo = heap_ver_max(heap);
        if(!encolado){
            encola_correctamente = false;
            break;
        }
        if(*maximo != *elemento){
            maximos_correctos = false;
            break;
        }
    }
    print_test("Los elementos se encolaron correctamente", encola_correctamente);
    print_test("los maximos se respetan correctamente", maximos_correctos);
    print_test("El heap no esta vacio", heap_esta_vacio(heap) == false);

    /* Desencolo elementos */
    
    bool desencola_correctamente = true;
    bool desencola_maximo = true;

    for (unsigned j = 0; j < volumen; j++){
        int* maximo = heap_ver_max(heap);
        int* desencolado = heap_desencolar(heap);
        if(!desencolado){
            desencola_correctamente = false;
            break;
        }
        if (*maximo != *desencolado){
            desencola_maximo = false;
            break;
        }
        free(desencolado);
    }
    print_test("los elementos se desencolaron correctamente", desencola_correctamente);
    print_test("los elementos desencolados son los maximos", desencola_maximo);
    print_test("el heap se encuentra vacio", heap_esta_vacio(heap) == true);
    print_test("la cantidad de elementos en el heap es 0", heap_cantidad(heap) == 0);


    /* Destruyo el heap */
    heap_destruir(heap, NULL);

    print_test("el heap se destruyo correctamente", true);

}

void pruebas_crear_arr_vacio(void){
    printf("\n----- Inicio de pruebas sobre Constructor vacio ------\n");
    void* arreglo[] = {NULL};

    heap_t* heap = heap_crear_arr(arreglo, 0, cmp_func);
    print_test("Creando heap vacio distinto de NULL", heap != NULL);

    print_test("Heap esta vacio", heap_esta_vacio(heap) == true);
    print_test("La cantidad de elementos en el heap es 0", heap_cantidad(heap) == 0);
    print_test("Ver el maximo del heap es NULL", heap_ver_max(heap) == NULL);
    print_test("Desencolar en un heap vacio es NULL", heap_desencolar(heap) == NULL);

    /* Destruyo el heap */
    heap_destruir(heap, NULL);
    print_test("El heap se destruyo correctamente", true);
    
}


void pruebas_heap_alumno(void){
    prueba_heap_vacio();
    pruebas_heap_sort();
    pruebas_heap_destruir_no_NULL();
    pruebas_varias_heap();
    pruebas_crear_arr_vacio();
    heap_pruebas_volumen(3000);
    heap_pruebas_volumen(6000);
    heap_pruebas_volumen(12000);
    heap_pruebas_volumen(20000);
}