#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include "abb.h"


/******ESTRUCTURAS*******/

typedef struct nodo_abb {
	struct nodo_abb* izq;
	struct nodo_abb* der;
	char* clave;
	void* dato;
}nodo_abb_t;



struct abb {
	nodo_abb_t* raiz;
	size_t cant;
	abb_destruir_dato_t destruir;
	abb_comparar_clave_t comparar;
};

//falta struct del iterador

/*******************************/


/******FUNCIONES AUX PARA IMPLEMENTACION DE ABB**********/

nodo_abb_t* _buscar_nodo_padre(nodo_abb_t* nodo, const char* clave, abb_comparar_clave_t cmp);

nodo_abb_t* _buscar_nodo(nodo_abb_t* nodo, const char* clave, abb_comparar_clave_t cmp);

nodo_abb_t* crear_nodo(const char* clave, void* dato);

void insertar_nodo(abb_t* arbol, nodo_abb_t* nodo_a_insertar);

void destruir_estructura_arbol(nodo_abb_t* nodo, abb_destruir_dato_t destruir)

/********************************************************/



/***PRIMITIVAS ABB **/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir){
	abb_t* arbol = malloc(sizeof(abb_t));
	if(!arbol){
		return NULL;
	}
	arbol->raiz = NULL;
	arbol->cant = 0;
	arbol->destruir = destruir;
	arbol->comparar = cmp;

	return arbol;
}


bool abb_guardar(abb_t* arbol, const char* clave, void* dato){
	nodo_abb_t* nuevo_nodo = crear_nodo(clave, dato);
	if(!nuevo_nodo || !arbol){
		return false;
	}
	if(!arbol->raiz){
		arbol->raiz = nuevo_nodo;
		arbol->cant++;
		return true;
	}
	nodo_abb_t* esta_nodo_en_abb = _buscar_nodo(arbol->raiz, clave, arbol->comparar);
	if(esta_nodo_en_abb){
		if(arbol->destruir != NULL){
			arbol->destruir(esta_nodo_en_abb->dato);
		}
		arbol->dato = dato;
	}
	else{
		insertar_nodo(arbol, nuevo_nodo);
	}
	arbol->cant++;				
	return true;

}

bool abb_pertenece(abb_t* arbol, char* clave){
	nodo_abb_t* pertenece = _buscar_nodo(arbol->raiz, clave, arbol->comparar);
	if(!pertenece){
		return false;
	}
	return true;
}


void* abb_obtener(abb_t* arbol, const char* clave){
	nodo_abb_t* obtener_nodo = _buscar_nodo(arbol->raiz, clave, arbol->comparar);
	if(!obtener_nodo){
		return NULL;
	}
	return obtener_nodo->dato;

}

void abb_destruir(abb_t* arbol){
	if (!arbol) return;
	destruir_estructura_arbol(arbol->raiz, arbol->destruir);
	free(arbol);
}

size_t abb_cantidad(abb_t* arbol){
	return arbol->cant;
}


/************* FUNCIONES AUXILIARES **********/

nodo_abb_t* _buscar_nodo(nodo_abb_t* nodo, const char* clave, abb_comparar_clave_t cmp){
	if(!nodo) 
		return NULL;
	if (cmp(nodo->clave, clave) == 0){
		return nodo;
	}
	else if(cmp(nodo->clave, clave) > 0)
		return _buscar_nodo(nodo->izq, clave, cmp);
	else
		return _buscar_nodo(nodo->der, clave, cmp);
}


nodo_abb_t* crear_nodo(const char* clave, void* dato){
	nodo_abb_t* nodo = malloc(sizeof(nodo_abb_t));
	if(!nodo){
		return NULL;
	}
	char* clave_copiada = strdup(clave);
	if(!clave_copiada) return NULL;
	nodo->izq = NULL;
	nodo->der = NULL;
	nodo->clave = clave_copiada;
	nodo->dato = dato;
	return nodo;
}


nodo_abb_t* _buscar_nodo_padre(nodo_abb_t* nodo, const char* clave, abb_comparar_clave_t cmp){
	
	if(cmp(nodo->clave, clave) > 0 && nodo->izq != NULL && cmp(nodo->izq, clave) != 0){
		_buscar_nodo_padre(nodo->izq, clave, cmp);
	}
	else if(cmp(nodo->clave, clave) < 0 && nodo->der != NULL && cmp(nodo->der, clave) != 0){
		_buscar_nodo_padre(nodo->der, clave, cmp);
	}
	else{
		return nodo;
	}
}

void insertar_nodo(abb_t* arbol, nodo_abb_t* nodo_a_insertar){
	nodo_abb_t* nodo_padre = _buscar_nodo_padre(arbol->raiz, clave, arbol->comparar);
	if(arbol->comparar(nodo_padre, clave) > 0)
		nodo->padre->izq = nuevo_nodo;
	else
		nodo->padre->der = nuevo_nodo; 
}

void destruir_estructura_arbol(nodo_abb_t* nodo, abb_destruir_dato_t destruir){
	if (!nodo) return;
	destruir_estructura_arbol(nodo->izq, destruir);
	destruir_estructura_arbol(nodo->der, destruir);
	if (destruir != NULL){
		destruir(nodo->dato);
	}
	free(clave);
}