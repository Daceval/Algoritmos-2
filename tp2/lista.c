#include "lista.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define VACIO 0


typedef struct nodo {
	void* dato;
	struct nodo *sig;
} nodo_t;

struct lista {
    nodo_t* primero;
    nodo_t* ultimo;
    size_t largo;
};

struct lista_iter {
	lista_t *lista;
	nodo_t *actual;
	nodo_t *anterior;
};

nodo_t* nodo_crear(void* dato);

bool lista_iter_al_principio(lista_iter_t *iter);

/* *****************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

lista_t* lista_crear(void){
	lista_t *lista = malloc(sizeof(lista_t));	
	if(lista == NULL){
		free(lista);
		return NULL;
	}
	lista->primero = NULL;
	lista->ultimo = NULL;
	lista->largo = VACIO;
	return lista;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void*)){
	nodo_t *nodo_aux;
	while(!lista_esta_vacia(lista)){
		nodo_aux = lista->primero;
		if(destruir_dato != NULL)
			destruir_dato(nodo_aux->dato);
		lista->primero = lista->primero->sig;
		free(nodo_aux);
	}
	free(lista);
}

size_t lista_largo(const lista_t *lista){
	return lista->largo;
}

bool lista_esta_vacia(const lista_t *lista){
	return !lista->primero;
}

void* lista_ver_primero(const lista_t *lista){
	if(lista_esta_vacia(lista))
		return NULL;
	return lista->primero->dato;
}

void* lista_ver_ultimo(const lista_t *lista){
	if(lista_esta_vacia(lista))
		return NULL;
	return lista->ultimo->dato;
}

void* lista_borrar_primero(lista_t *lista){
	if(lista_esta_vacia(lista))
		return NULL;
	void* dato;
	nodo_t *aux = lista->primero;
	dato = lista->primero->dato;
	lista->primero = lista->primero->sig;
	/*Si el primero de la lista ahora apunta a NULL, quiere decir que estaba
	apuntando al ultimo, entonces cambio ultimo a NULL. */
	if(lista->primero == NULL)
		lista->ultimo = NULL;
	lista->largo--;
	free(aux);
	return dato;
}

bool lista_insertar_primero(lista_t *lista, void *dato){
	nodo_t *nodo = nodo_crear(dato);
	if(!nodo)
		return false;
	if(lista_esta_vacia(lista)){
		lista->ultimo = nodo;
	} else {
		nodo->sig = lista->primero;
	}
	lista->primero = nodo;
	lista->largo++;
	return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){
	nodo_t *nodo = nodo_crear(dato);
	if(!nodo)
		return false;
	if(lista_esta_vacia(lista)){
		lista->primero = nodo;
	} else {
		lista->ultimo->sig = nodo;
	}
	lista->ultimo = nodo;
	lista->largo++;
	return true;
}

/* ******************************************************************
 *                  PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
	lista_iter_t* iter = lista_iter_crear(lista);
	while(!lista_iter_al_final(iter) && visitar(lista_iter_ver_actual(iter), extra)){
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
}


/* ******************************************************************
 *                  PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

lista_iter_t *lista_iter_crear(lista_t *lista){
	lista_iter_t* iter = malloc(sizeof(lista_iter_t));
	if(!iter)
		return NULL;
	iter->lista = lista;
	iter->actual = iter->lista->primero;
	iter->anterior = NULL;
	return iter;
}

bool lista_iter_avanzar(lista_iter_t *iter){
	if(lista_iter_al_final(iter))
		return false;
	iter->anterior = iter->actual;
	iter->actual = iter->actual->sig;
	return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){
	if(lista_iter_al_final(iter)){
		return NULL;
	}
	return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter){
	return !iter->actual;
}

void lista_iter_destruir(lista_iter_t *iter){
	free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){
	nodo_t *nodo = nodo_crear(dato);
	if(!nodo)
		return false;
	nodo->sig = iter->actual;
	iter->actual = nodo;
	iter->lista->largo+=1;
	if(!iter->lista->primero){
		iter->lista->primero = nodo;
		iter->lista->ultimo = nodo;
	}	
	if(!nodo->sig && !lista_iter_al_principio(iter)){
		iter->lista->ultimo = nodo;
		iter->anterior->sig = nodo;
	}
	if(lista_iter_al_principio(iter)){
		iter->lista->primero = nodo;

	} else {
		iter->anterior->sig = nodo;
	}
	return true;
}

void *lista_iter_borrar(lista_iter_t *iter){
	if(lista_iter_al_final(iter))
		return NULL;
	nodo_t *aux = iter->actual;
	void *dato = iter->actual->dato;
	if(!lista_iter_al_principio(iter)){
		iter->anterior->sig = iter->actual->sig;
	} else {
		iter->lista->primero = iter->actual->sig;
	}
	if(iter->actual == iter->lista->ultimo)
		iter->lista->ultimo = iter->anterior;
	iter->actual = iter->actual->sig;
	iter->lista->largo-=1;
	free(aux);
	return dato;
}


/* FUNCIONES AUXILIARES PARA IMPLEMENTACIÓN */

nodo_t* nodo_crear(void* dato){
	nodo_t *nuevo_nodo = malloc(sizeof(nodo_t));
	if(nuevo_nodo == NULL)
		return NULL;
	nuevo_nodo->dato = dato;
	nuevo_nodo->sig = NULL;
	return nuevo_nodo;
}

bool lista_iter_al_principio(lista_iter_t *iter){
	return !iter->anterior;
}





