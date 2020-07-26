#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "csv.h"
#include "lista.h"
#include "hash.h"
#include "funciones_tp2.h"
#include "strutil.h"
#include "mensajes.h"

#define PACIENTES 1
#define DOCTORES 0

#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"

clinica_t* init(char** argv);

void cmd_error_check(const char* comando, const char** parametros, clinica_t* clinica);




#include <stdio.h>
#include <string.h>

typedef struct clinica{
	abb_t* abb_docs;
	abb_t* abb_pacs;
	heap_t* turnos_reg;
	cola_t* turnos_urg;
	hash_t* tabla_esps;
}clinica_t;

typedef struct paciente{
	char* nombre;
	size_t año;
}paciente_t;

typedef struct doctor{
	char* nombre;
	char* esp;
}doctor_t;

// typedef struct turno{
// 	paciente_t* p;
// 	doctor_t* d;
// 	size_t urgencia;

// }turno_t;


int main(int argc, char** argv){
	clinica_t clinica = init(argv);
	while(1){
		procesar_entrada();
	}

}

clinica_t* init(char** argv){
	abb_t* arbol_pacientes = csv_crear_estructura(argv[1], doctor_wrapper, NULL);
	abb_t* arbol_doctores = csv_crear_estructura(argv[2], paciente_wrapper, NULL);
	hash_t* hash = hash_crear(NULL);
	if(!hash){

	}
	abb_iter_t* iter = abb_iter_in_crear(arbol_doctores);
	while(!abb_iter_in_al_final(iter)){
		doctor_t* doc = abb_obtener(abb_iter_in_ver_actual(iter));
		hash_guardar(hash, doc->esp, doc);
		abb_iter_in_avanzar(iter);
	}
	abb_iter_in_destruir(iter);
	cola_t* cola = cola_crear();
	heap_t* heap = heap_crear();
	clinica_t clinica = malloc(sizeof(clinica_t));
	if(!clinica){

	}
	clinica->abb_docs = arbol_doctores;,
	clinica->abb_pacs = arbol_pacientes;
	clinica->turnos_reg = heap;
	clinica->turnos_urg = cola;
	clinica->tabla_esps = hash;
	return clinica;
}

void* paciente_wrapper(char** params, void* extra) {
	return paciente_crear(params[0], atoi(params[1]));
}

void* doctor_wrapper(char** params, void* extra) {
	return doctor_crear(params[0], params[1]);
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
	d->esp = esp;
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


void cmd_error_check(const char* comando, const char** parametros, clinica_t* clinica) {
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {

	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {

	} else if (strcmp(comando, COMANDO_INFORME) == 0) {

	} else {
		printf(ENOENT_CMD, comando);
	}
}

bool pedir_turno_error_check(const char** parametros, clinica_t* clinica){
	bool ok = true;
	if(!abb_pertenece(clinica->abb_pacs, parametros[0])){
		printf(ENOENT_PACIENTE, parametros[0]);
		ok = false;
	}
	if(!hash_pertenece(clinica->tabla_esps, parametros[1])){
		printf(ENOENT_ESPECIALIDAD, parametros[1]);
		ok = false;
	}
	if(strcmp(parametros[2], "URGENTE") == 0 && ok){
		if(!cola_encolar(clinica->turnos_urg, abb_obtener(clinica->abb_pacs, parametros[0])))
			ok = false;
	} else if (strcmp(parametros[2], "REGULAR") == 0 && ok){
		if(!heap_encolar(clinica->turnos_reg, abb_obtener(clinica->abb_pacs, parametros[0])))
			ok = false;;
	} else{
		printf(ENOENT_URGENCIA, parametros[2]);
	}
	if(ok){
		printf(PACIENTE_ENCOLADO, parametros[0]);
		printf(CANT_PACIENTES_ENCOLADOS, );
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
		char** campos = split(linea, ':');
		if (campos[1] == NULL) {
			printf(ENOENT_FORMATO, linea);
			free_strv(campos);
			continue;	
		}
		char** parametros = split(campos[1], ',');
		cmd_error_check(campos[0], parametros);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
}