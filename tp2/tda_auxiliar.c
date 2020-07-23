#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define REGULAR "REGULAR"
#define URGENTE "URGENTE"



typedef struct datos_paciente 
{
	char* nombre_paciente;
	char* antiguedad;

}datos_paciente_t;



typedef struct pacientes
{
	heap_t* regulares;
	cola_t* urgentes;

}pacientes_t;


pacientes_t* pacientes_crear(){
	pacientes_t* pacientes = malloc(sizeof(pacientes_t));
	if (!pacientes) return NULL;
	pacientes->regulares = heap_crear();
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


bool atender_pacientes(pacientes_t* pacientes, char* urgencia){
	if(!pacientes){
		return false;
	}
	if(!strcmp(urgencia, REGULAR)){
		heap_desencolar(pacientes->regulares);
	}else{
		cola_desencolar(pacientes->urgentes);
	}

	return true;
}


bool pedir_turno(pacientes_t* pacientes, char* urgencia, char* nombre, char* antiguedad){
	datos_paciente_t* datos = malloc(sizeof(datos_paciente_t));
	if(!datos) return false;
	
	datos->nombre_paciente = strdup(nombre);
	datos->antiguedad = strdup(antiguedad);	
	if (!strcmp(urgencia, URGENTE)){
		cola_encolar(pacientes->urgentes, datos);
	}else{
		heap_encolar(pacientes->regulares, datos);
	}

	return true;
}

void pacientes_destruir(pacientes_t* pacientes){
	
	heap_destruir(pacientes->regulares);
	cola_destruir(pacientes->urgentes);
	free(pacientes);
}