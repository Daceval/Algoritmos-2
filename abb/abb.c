#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include "abb.h"
#include "pila.h"


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

struct abb_iter{
	pila_t* pila;
}


/******FUNCIONES AUX PARA IMPLEMENTACION DE ABB**********/

nodo_abb_t* _buscar_nodo_padre(nodo_abb_t* nodo, const char* clave, abb_comparar_clave_t cmp);

nodo_abb_t* _buscar_nodo(nodo_abb_t* nodo, const char* clave, abb_comparar_clave_t cmp);

nodo_abb_t* crear_nodo(const char* clave, void* dato);

nodo_abb_t* buscar_reemplazante(nodo_abb_t* nodo);

void insertar_nodo(abb_t* arbol, nodo_abb_t* nodo_a_insertar);

void destruir_estructura_arbol(nodo_abb_t* nodo, abb_destruir_dato_t destruir);

void llenar_pila(pila_t* pila, nodo_abb_t* nodo);

void iterar_in_order(nodo_abb_t* nodo, bool visitar(const char*, void*, void*), void* extra, bool* avanzar);

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

void *abb_borrar(abb_t *arbol, const char *clave){
	if(!abb_pertenece(arbol, clave))
		return NULL;
	void* dato = NULL;
	nodo_abb_t* nodo_aux = _buscar_nodo(arbol->raiz, clave, arbol->comparar);
	bool ok = nodo_aux->izq == NULL || nodo_aux->der == NULL;
	if(ok){
		nodo_abb_t* hijo = NULL;
		if(ok&nodo_aux->izq)
			hijo = nodo_aux->izq;
		if(ok&nodo_aux->der)
			hijo = nodo_aux->der;
		nodo_abb_t* padre_nodo_aux = _buscar_nodo_padre(arbol->raiz, clave, arbol->comparar)
		if(padre_nodo_aux->izq == nodo_aux)
			padre_nodo_aux->izq = hijo;
		else
			padre_nodo_aux->der = hijo;
		dato = nodo_aux->dato;
		free(nodo_aux->clave);
		free(nodo_aux);
		return dato;
	}
	nodo_abb_t* reemplazante = buscar_reemplazante(nodo_aux);
	free(nodo_aux->clave);
	dato = nodo_aux->dato;
	nodo_aux->clave = strdup(reemplazante->clave);
	nodo_aux->dato = abb_borrar(arbol, reemplazante->clave);
	return dato;
}

bool abb_pertenece(abb_t* arbol, char* clave){
	nodo_abb_t* pertenece = _buscar_nodo(arbol->raiz, clave, arbol->comparar);
	return !pertenece ? false : true;
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


/***********PRIMITIVAS ITERADOR EXTERNO *************/

abb_iter_t* abb_iter_in_crear(const abb_t* arbol ){
	if(!arbol){
		return NULL;
	}
	abb_iter_t* iter = malloc(sizeof(abb_iter_t));
	if(!iter){
		return NULL;
	}
	iter->pila = pila_crear();
	if(!iter->pila){
		free(iter);
		return NULL;
	}
	llenar_pila(pila, arbol->raiz);
	return iter;
}


bool abb_iter_in_avanzar(abb_iter_t* iter){
	if(abb_iter_in_al_final(iter)){
		return false;
	}
	nodo_abb_t* nodo = (nodo_abb_t*) pila_desapilar(iter->pila);
	nodo_abb_t* nodo_derecho = nodo->der;
	if(nodo_derecho){
		llenar_pila(iter->pila, nodo_derecho);
	}
	return true;
}

const char* abb_iter_in_ver_actual(const abb_iter_t* iter){
	nodo_abb_t* nodo = (nodo_abb_t*)pila_ver_tope(iter->pila);
	return nodo->clave;
}


bool abb_iter_in_al_final(const abb_iter_t* iter){
	return pila_esta_vacia(iter->pila) ? true : false;
}


void abb_iter_in_destruir(abb_iter_t* iter){
	pila_destruir(iter->pila);
	free(iter);
}



/*** PRIMITIVA ITERADOR INTERNO *****/

void abb_in_order(abb_t* arbol, bool visitar(const char*, void*, void*), void* extra){
	if(!arbol){
		return;
	}
	bool llamar_recursivamente = true;
	iterar_in_order(arbol->raiz, visitar, extra, &llamar_recursivamente);
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

nodo_abb_t* buscar_reemplazante(nodo_abb_t* nodo){
	nodo_abb_t* reemplazante = nodo->der;
	while(reemplazante->izq != NULL)
		reemplazante = reemplazante->izq;
	return reemplazante;
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

void llenar_pila(pila_t* pila, nodo_abb_t* nodo){
	if(!nodo){
		return;
	}
	pila_apilar(pila, nodo);
	llenar_pila(pila, nodo->izq);
}

void iterar_in_order(nodo_abb_t* nodo, bool visitar(const char*, void*, void*), void* extra, bool* avanzar){
	if (!nodo || !*avanzar){
		return;
	}
	iterar_in_order(nodo->izq, vistar, extra, avanzar);
	if (*avanzar){
		*avanzar = visitar(nodo->clave, nodo->dato, extra);
		if(!*avanzar){
			return;
		}
	}
	iterar_in_order(nodo->der, visitar, extra, avanzar);
}