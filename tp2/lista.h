#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stdlib.h>


/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

struct lista;
typedef struct lista lista_t;

typedef struct lista_iter lista_iter_t;


/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista.
// Post: devuelve una nueva lista vacía.
lista_t* lista_crear(void);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void (*destruir_dato)(void*));

// Devuelve verdadero si la lista no tiene elementos enlistados, false en caso contrario.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Devuelve la cantidad de elementos de la lista.
// Pre: la lista fue creada.
// Post: Se devuelve el largo de la lista. NULL en caso de que esté vacía.
size_t lista_largo(const lista_t *lista);

// Obtiene el valor del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void* lista_ver_primero(const lista_t *lista);

// Obtiene el valor del último elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del último, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el último elemento de la lista, cuando no está vacía.
void* lista_ver_ultimo(const lista_t *lista);

// Saca el primer elemento de la lista. Si la lista tiene elementos, se quita el
// primero de la lista, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del primer elemento anterior, la lista
// contiene un elemento menos, si la lista no estaba vacía.
void* lista_borrar_primero(lista_t *lista);

// Inserta un valor en la primera posición de la lista. Devuelve true en caso
// de que se haya podido insertar y false en caso contrario.
// Pre: la lista fue creada.
// Post: se agregó un elemento al principio de la lista. La lista tiene un elemento más.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Inserta un valor en la última posición de la lista. Devuelve true en caso
// de que se haya podido insertar y false en caso contrario.
// Pre: la lista fue creada.
// Post: se agregó un elemento al final de la lista. La lista tiene un elemento más.
bool lista_insertar_ultimo(lista_t *lista, void* dato);

/* ******************************************************************
 *                  PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);



/* ******************************************************************
 *                  PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

//Crea un iterador.
//Post: devuelve un iterador que apunta al primer elemento de la lista.
lista_iter_t *lista_iter_crear(lista_t *lista);

//Hace que el iterador avance una posición. Devuelve true en caso de poder hacerlo
//y devuelve false en caso contrario.
//Pre: el iterador fue creado.
//Post: el iterador se encuentra en una posición más adelante.
bool lista_iter_avanzar(lista_iter_t *iter);

//Devuelve el valor al cual el iterador esta apuntando.
//Pre: el iterador fue creado
//Post: se devolvió el valor al cual está aputando el iterador.
void *lista_iter_ver_actual(const lista_iter_t *iter);

//Devuelve true si el iterador se encuentra al final, es decir, que apunta a
//NULL. Devuelve false en caso de estar apuntando a un elemento valido de la lista
//Pre: el iterador fue creado.
bool lista_iter_al_final(const lista_iter_t *iter);

//Destruye el iterador.
//Pre: el iterador fue creado.
//Post: el iterador fue destruido.
void lista_iter_destruir(lista_iter_t *iter);

//Inserta el dato entre el elemento al que está apuntando el iterador y el siguiente.
//Devuelve true en caso de que se haya podido insertar y false en caso contrario.
//Pre: el iterador fue creado.
//Post: se insertó el dato en la lista. La lista tiene un elemento más.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

//Borra el elemento que está siendo apuntado por el iterador. Devuelve el dato 
//que está siendo borrado de la lista. Si la lista esta vacia, devuelve NULL.
//Pre: el iterador fue creado.
//Post: se devolvió el dato que fue borrado de la lista. La lista tiene un elemento menos. 
void *lista_iter_borrar(lista_iter_t *iter);



/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

// Realiza pruebas sobre la implementación del alumno.
//
// Las pruebas deben emplazarse en el archivo ‘pruebas_alumno.c’, y
// solamente pueden emplear la interfaz pública tal y como aparece en lista.h
// (esto es, las pruebas no pueden acceder a los miembros del struct lista).
//
// Para la implementación de las pruebas se debe emplear la función
// print_test(), como se ha visto en TPs anteriores.
void pruebas_lista_alumno(void);

#endif // lista_H
