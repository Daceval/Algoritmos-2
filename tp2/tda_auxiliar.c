#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define REGULAR "REGULAR"
#define URGENTE "URGENTE"
#include "heap.h"
#include "cola.h"



typedef struct cola_pacientes {
	heap_t* regulares;
	cola_t* urgentes;

}cola_pacientes_t;



cola_pacientes_t* cola_pacientes_crear(cmp_func_t cmp){  //pasar_una_funcion_comparacion
	pacientes_t* pacientes = malloc(sizeof(pacientes_t));
	if (!pacientes) return NULL;
	pacientes->regulares = heap_crear(cmp);
	if (!pacientes->regulares){
		free(pacientes);
		return NULL;
	}

	pacientes->urgentes = cola_crear();
	if(!pacientes->urgentes){
		free(pacientes);
		free(pacientes->regulares);
		return NULL;
	}
	return pacientes;
}



void* desencolar_paciente(pacientes_t* pacientes, char* urgencia){

	if(!pacientes){
		return false;
	}
	if(!strcmp(urgencia, REGULAR) && !heap_esta_vacio(pacientes->regulares)){
		heap_desencolar(pacientes->regulares);
	}
	else if (!strcmp(urgencia, URGENTE) && !cola_esta_vacia(pacientes->urgentes)){
		cola_desencolar(pacientes->urgentes);
	}else{
		//no hay pacientes en espera...
	}
	pacientes->pacientes_atendidos++;
	return true;
}


size_t cant_pacientes_regular(cola_pacientes_t* pacientes){
	heap_cantidad(pacientes->regulares);
}


size_t cant_pacientes_urgentes(cola_pacientes_t* pacientes){
	cola_cantidad(pacientes->urgentes);
}



bool encolar_turno(cola_pacientes_t* pacientes, char* urgencia, void* datos){
	if(!datos) return false;
	
	if (!strcmp(urgencia, URGENTE)){
		cola_encolar(pacientes->urgentes, datos);
	}else{
		heap_encolar(pacientes->regulares, datos);
	}

	return true;
}

void pacientes_destruir(cola_pacientes_t* pacientes){
	
	heap_destruir(pacientes->regulares);
	cola_destruir(pacientes->urgentes);
	free(pacientes);
} 

