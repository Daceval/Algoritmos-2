#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "csv.h"
#include "lista.h"
#include "abb.h"
#include "hash.h"
#include "cola.h"
#include "funciones_tp2.h"
#include "strutil.h"
#include "mensajes.h"

#define PACIENTES 1
#define DOCTORES 0

#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"

#define CANT_PARAMS_PEDIR_TURNO 2
#define CANT_PARAMS_ATENDER 1

clinica_t* init(char** argv);

void cmd_load(const char* comando, const char** parametros, clinica_t* clinica);

typedef enum {	
	LIBRE,
	OCUPADO
}estado_t;

typedef struct clinica{
	abb_t* abb_docs;
	abb_t* abb_pacs;
	hash_t* tabla_esps;
}clinica_t;

typedef struct paciente{
	char* nombre;
	size_t año;
}paciente_t;

typedef struct doctor{
	char* nombre;
	char* esp;
	size_t pac_atendidos;
	estado_t estado;
}doctor_t;


int main(int argc, char** argv){
	clinica_t clinica = init(argv);
	while(1){
		procesar_entrada();
	}

}


clinica_t* init(char** argv){
	abb_t* arbol_pacientes = csv_crear_estructura(argv[1], doctor_wrapper, doctor_destruir);
	abb_t* arbol_doctores = csv_crear_estructura(argv[2], paciente_wrapper, paciente_destruir);
	hash_t* hash = hash_crear(NULL);
	abb_iter_t* iter = abb_iter_in_crear(arbol_doctores);
	if(!hash || !iter){
		abb_destruir(arbol_pacientes);
		abb_destruir(arbol_doctores);
		return NULL;
	}
	while(!abb_iter_in_al_final(iter)){
		doctor_t* doc = abb_obtener(abb_iter_in_ver_actual(iter));
		hash_guardar(hash, doc->esp, doc);
		abb_iter_in_avanzar(iter);
	}
	abb_iter_in_destruir(iter);
	cola_t* cola = cola_crear();
	//Agregar funcion de comparacion
	heap_t* heap = heap_crear();
	if(!cola || !heap){
		abb_destruir(arbol_pacientes);
		abb_destruir(arbol_doctores);
		hash_destruir(hash);
		return NULL;
	}
	clinica_t clinica = malloc(sizeof(clinica_t));
	if(!clinica){
		abb_destruir(arbol_pacientes);
		abb_destruir(arbol_doctores);
		cola_destruir(cola);
		heap_destruir(heap);
		hash_destruir(hash);
		return NULL;
	}
	clinica->abb_docs = arbol_doctores;,
	clinica->abb_pacs = arbol_pacientes;
	clinica->tabla_esps = hash;
	return clinica;
}

void* paciente_wrapper(char** params) {
	return paciente_crear(params[0], atoi(params[1]));
}

void* doctor_wrapper(char** params) {
	return doctor_crear(params[0], params[1]);
}

void doctor_destruir(doctor_t* d){
	free(d->nombre);
	free(d->esp);
	free(d);
}

void paciente_destruir(paciente_t* p){
	free(p->nombre);
	free(p);
}

doctor_t* doctor_crear(const char* nombre, char* esp) {
	doctor_t* d = malloc(sizeof(doctor_t));
	if (!d) 
		return NULL;
	d->nombre = strdup(nombre);
	if (!d->nombre) {
		free(d);
		return NULL;
	}
	d->esp = strdup(esp);
	return i;
}

paciente_t* paciente_crear(const char* nombre, size_t año_i) {
	paciente_t* p = malloc(sizeof(paciente_t));
	if (!p) 
		return NULL;
	p->nombre = strdup(nombre);
	if (!p->nombre) {
		free(p);
		return NULL;
	}
	p->año = año_i;
	return p;
}

bool cant_params_check(char* cmd, size_t cant_params, const char** parametros){
	size_t i;
	for(i = 0; parametros[i] != NULL; i++);
	if (i != cant_params){
		printf(ENOENT_PARAMS, cmd);
		return false;
	}
	return true;
}

void cmd_load(const char* comando, const char** parametros, clinica_t* clinica) {
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {
		if(cant_params_check(COMANDO_PEDIR_TURNO, CANT_PARAMS_PEDIR_TURNO, parametros))
			pedir_turno(parametros, clinica);
	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {
		if(cant_params_check(COMANDO_ATENDER, CANT_PARAMS_ATENDER, parametros))
			atender_siguiente();
	} else if (strcmp(comando, COMANDO_INFORME) == 0) {
		
	} else {
		printf(ENOENT_CMD, comando);
	}
}

void pedir_turno(const char** parametros, clinica_t* clinica){
	cola_pacientes_t* turno_especialidad = hash_obtener(clinica->abb_pacs, parametros[1]);
	paciente_t* turno_paciente = abb_obtener(clinica->abb_pacs, parametros[0]);
	bool ok = true;
	if(!turno_paciente){
		printf(ENOENT_PACIENTE, parametros[0]);
		ok = false;
	}
	if(!turno_especialidad){
		printf(ENOENT_ESPECIALIDAD, parametros[1]);
		ok = false;
	}
	if(strcmp(parametros[2], "URGENTE") == 0 && ok){
		if(!cola_encolar(turno_especialidad->urgentes, turno_paciente))
			ok = false;
	} else if (strcmp(parametros[2], "REGULAR") == 0 && ok){
		if(!heap_encolar(turno_especialidad->regulares, turno_paciente))
			ok = false;
	} else if (strcmp(parametros[2], "REGULAR") != 0 || strcmp(parametros[2], "URGENTE") != 0){
		printf(ENOENT_URGENCIA, parametros[2]);
		ok = false;
	}
	if(ok){
		printf(PACIENTE_ENCOLADO, parametros[0]);
		printf(CANT_PACIENTES_ENCOLADOS, turno_especialidad->cant_espera, parametros[1]);
		//SUMAR UNO A LA CANTIDAD;
	}
}

void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

void procesar_entrada() {
	char* linea = NULL;
	size_t c = 0;
	while (getline(&linea, &c, stdin) > 0) {
		eliminar_fin_linea(linea);
		if(strcmp(linea, "exit") == 0){
			printf("FIN\n");
			return;
		}
		char** campos = split(linea, ':');
		if (campos[1] == NULL) {
			printf(ENOENT_FORMATO, linea);
			free_strv(campos);
			continue;	
		}
		char** parametros = split(campos[1], ',');
		cmd_load(campos[0], parametros);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
}

abb_t* csv_crear_abb(const char* ruta_csv, void* (*creador) (char**), void (*destruir_dato) (void*)) {
	FILE* archivo = fopen(ruta_csv, "r");
	if (!archivo) {
		return NULL;
	}
	
	abb_t* abb = abb_crear(strcmp, destruir_dato);
	if (!abb) {
		fclose(archivo);
		return NULL;
	}
z
	char* linea = NULL;
	size_t c = 0;
	while (getline(&linea, &c, archivo) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, SEPARADOR);
		abb_guardar(abb, campos[0],  creador(campos));
		free_strv(campos);
	}
	free(linea);
	fclose(archivo);
	return abb;
}