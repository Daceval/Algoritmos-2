#include "cola.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


typedef struct nodo {
	void* dato;
	struct nodo *sig;
} nodo_t;

struct cola {
    nodo_t* primero;
    nodo_t* ultimo;
};

nodo_t* nodo_crear(void* dato);

/* *****************************************************************
 *                    PRIMITIVAS DE LA COLA
 * *****************************************************************/

// ...


cola_t* cola_crear(void){
	cola_t *cola = malloc(sizeof(cola_t));	
	if(cola == NULL)
		return NULL;
	cola->primero = NULL;
	cola->ultimo = NULL;
	return cola;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void*)){
	void* dato;
	while(!cola_esta_vacia(cola)){
		dato = cola_desencolar(cola);
		if(destruir_dato != NULL)
			destruir_dato(dato);
	}
	free(cola);
}

bool cola_esta_vacia(const cola_t *cola){
	return !cola->primero;
}

bool cola_encolar(cola_t *cola, void* valor){
	if(!cola)
		return false;
	nodo_t *nodo = nodo_crear(valor);
	if(!nodo)
		return false;
	if(cola_esta_vacia(cola)){
		cola->primero = nodo;
	} else {
		cola->ultimo->sig = nodo;
	}
	cola->ultimo = nodo;
	return true;
}

void* cola_ver_primero(const cola_t *cola){
	if(cola_esta_vacia(cola))
		return NULL;
	return cola->primero->dato;
}

void* cola_desencolar(cola_t *cola){
	if(cola_esta_vacia(cola))
		return NULL;
	void* dato;
	nodo_t *aux = cola->primero;
	dato = cola->primero->dato;
	cola->primero = cola->primero->sig;
	/*Si el primero de la cola ahora apunta a NULL, quiere decir que estaba
	apuntando al ultimo, entonces cambio ultimo a NULL. */
	if(cola->primero == NULL)
		cola->ultimo = NULL;
	free(aux);
	return dato;
}


/* FUNCIONES AUXILIARES PARA IMPLEMENTACIÓN */

nodo_t* nodo_crear(void* dato){
	nodo_t *nuevo_nodo = malloc(sizeof(void*) + sizeof(nodo_t));
	if(!nuevo_nodo)
		return NULL;
	nuevo_nodo->dato = dato;
	nuevo_nodo->sig = NULL;
	return nuevo_nodo;
}




