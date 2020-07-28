#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#include "mensajes.h"
#include "csv.h"
#include "heap.h"
#include "lista.h"
#include "abb.h"
#include "hash.h"
#include "cola.h"
//#include "funciones_tp2.h"
#include "strutil.h"



#define PACIENTES 1
#define DOCTORES 0
#define ENTRADA_VACIA " "

#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"

#define PACIENTE_REGULAR "REGULAR"
#define PACIENTE_URGENTE "URGENTE"

#define CANT_PARAMS_PEDIR_TURNO 2
#define CANT_PARAMS_ATENDER 1
#define CANT_PARAMS_INFORME 2


typedef enum {TODOS_LOS_ELEM, SIN_MINIMO, SIN_MAXIMO, CON_MIN_Y_MAX} extremos_t;

typedef enum {LIBRE, OCUPADO} estado_t;

/************* DECLARACION DE ESTRUCTURAS *****************/


typedef struct clinica clinica_t;

typedef struct paciente paciente_t;

typedef struct doctor doctor_t;

typedef struct cola_pacientes cola_pacientes_t;

/********* DECLARACIONES DE FUNCIONES PARA TP2 ****************/

clinica_t* init(char** argv);

cola_pacientes_t* cola_pacientes_crear(cmp_func_t cmp);

doctor_t* doctor_crear(const char* nombre, char* esp);

paciente_t* paciente_crear(const char* nombre, size_t anio_i);

bool cant_params_check(char* cmd, size_t cant_params, const char** parametros);

void cmd_load(const char* comando, const char** parametros, clinica_t* clinica);

void eliminar_fin_linea(char* linea);

void procesar_entrada(clinica_t* clinica);

void pedir_turno(const char** parametros, clinica_t* clinica);

void informe(const char** parametros, clinica_t* clinica);

void atender_siguiente(const char** parametros, clinica_t* clinica);

void* paciente_wrapper(char** params);

void* doctor_wrapper(char** params);

void doctor_destruir(doctor_t* d);

void paciente_destruir(paciente_t* p);

void doctor_destruir_wrapper(void*);

void paciente_destruir_wrapper(void*);

void cola_pacientes_destruir(cola_pacientes_t* pacientes);

int func(const char* min, const char* max);

void iteracion_por_rangos(abb_t* abb, bool (*visitar) (const char*, void*, void*), void* extra, int cant_extremos, const char* min, const char* max);

bool funtion(const char* clave, void* valor, void* extra);

int comparar_pacientes(const void* a, const void* b);

/************ DEFINICION DE ESTRUCTURAS *******************/

struct clinica{
	abb_t* abb_docs;
	abb_t* abb_pacs;
	hash_t* tabla_esps;
};


struct paciente{
	char* nombre;
	size_t anio;
};


struct doctor{
	char* nombre;
	char* esp;
	size_t pac_atendidos;
	estado_t estado;
};


struct cola_pacientes {
	heap_t* regulares;
	cola_t* urgentes;
	size_t cantidad_en_espera;
	
};

int comparar_pacientes(const void* a, const void* b){
	paciente_t* dato1 = (paciente_t*)a;
	paciente_t* dato2 = (paciente_t*)b;
	if (dato1->anio <  dato2->anio){
		return 1;
	}
	else if (dato1->anio > dato2->anio){
		return -1; 
	}else{
		return 0;
	}
}

/****** DEFINICIONES DE FUNCIONES PARA TP2  ***********/



/* funcion visitar para iterador interno de abb */
bool funtion(const char* clave, void* valor, void* extra){
	size_t n = *(size_t*)extra;
	doctor_t* info_doctor = (doctor_t*)valor;
	printf(INFORME_DOCTOR, n, clave, info_doctor->esp, info_doctor->pac_atendidos); //fijarse si esta bien ingresar a la estructura del doctor...
	n++;
	return true;
}

/* funcion que devuelve un valor entero dependiendo del min y max ingresado */
int func(const char* min, const char* max){
	int cant_extremos = 0;
	if (!strcmp(min, ENTRADA_VACIA) && !strcmp(max, ENTRADA_VACIA)){
		cant_extremos = TODOS_LOS_ELEM;
	}
	else if(strcmp(min, ENTRADA_VACIA) != 0 && strcmp(max, ENTRADA_VACIA) != 0){
		cant_extremos = CON_MIN_Y_MAX;
	}else{
		cant_extremos = !strcmp(min, ENTRADA_VACIA) && strcmp(max, ENTRADA_VACIA) != 0 ? SIN_MINIMO : SIN_MAXIMO;
	}
	return cant_extremos;
}

/* dependiendo de la entrada ingresada, llama al iterador interno de abb con min y max correspondientes */
void iteracion_por_rangos(abb_t* abb, bool visitar (const char*, void*, void*), void* extra, int cant_extremos, const char* min, const char* max){

	switch(cant_extremos){
		
		case TODOS_LOS_ELEM:{
			char* inicio = devolver_menor_clave(abb);
			char* fin = devolver_mayor_clave(abb);
			abb_in_order_por_rangos(abb, visitar, extra, inicio, fin); // O(log N) + K
			break;
		}
		case SIN_MINIMO:{
			char* inicio = devolver_menor_clave(abb);
			abb_in_order_por_rangos(abb, visitar, extra, inicio, max);
			break;
		}
		case SIN_MAXIMO:{
			char* fin = devolver_mayor_clave(abb);
			abb_in_order_por_rangos(abb, visitar, extra, min, fin);
			break;
		}
		default: 
			abb_in_order_por_rangos(abb, visitar, extra, min, max);
			break;
	}
}

void* paciente_wrapper(char** params) {
	size_t anio = atoi(params[1]);
	if(anio == 0){
		printf(ENOENT_ANIO, params[1]);
		return NULL;
	}
	return paciente_crear(params[0], atoi(params[1]));
}


void* doctor_wrapper(char** params) {
	return doctor_crear(params[0], params[1]);
}

void doctor_destruir_wrapper(void* dato){
	doctor_destruir(dato);
}

void paciente_destruir_wrapper(void* dato){
	paciente_destruir(dato);
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

void cola_pacientes_destruir(cola_pacientes_t* pacientes){
	
	heap_destruir(pacientes->regulares, NULL); //pasar una funcion de destruccion, tiraba error al compilar
												//por eso puse NULL
	cola_destruir(pacientes->urgentes, NULL);
	free(pacientes);
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
	d->pac_atendidos = 0;
	d->estado = LIBRE;
	return d;
}

paciente_t* paciente_crear(const char* nombre, size_t anio_i) {
	paciente_t* p = malloc(sizeof(paciente_t));
	if (!p) 
		return NULL;
	p->nombre = strdup(nombre);
	if (!p->nombre) {
		free(p);
		return NULL;
	}
	p->anio = anio_i;
	return p;
}

cola_pacientes_t* cola_pacientes_crear(cmp_func_t cmp){ 
	cola_pacientes_t* pacientes = malloc(sizeof(cola_pacientes_t));
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


clinica_t* init(char** argv){
	abb_t* arbol_pacientes = csv_crear_abb(argv[1], doctor_wrapper, doctor_destruir_wrapper);
	abb_t* arbol_doctores = csv_crear_abb(argv[2], paciente_wrapper, paciente_destruir_wrapper);
	hash_t* hash = hash_crear(NULL);
	abb_iter_t* iter = abb_iter_in_crear(arbol_doctores);
	if(!hash || !iter){
		abb_destruir(arbol_pacientes);
		abb_destruir(arbol_doctores);
		return NULL;
	}
	while(!abb_iter_in_al_final(iter)){
		cola_pacientes_t* q_pacientes = cola_pacientes_crear(comparar_pacientes);
		doctor_t* doc = abb_obtener(arbol_doctores,abb_iter_in_ver_actual(iter));//corregido
		hash_guardar(hash, doc->esp, q_pacientes);
		abb_iter_in_avanzar(iter);
	}
	abb_iter_in_destruir(iter);

	clinica_t* clinica = malloc(sizeof(clinica_t));
	if(!clinica){
		abb_destruir(arbol_pacientes);
		abb_destruir(arbol_doctores);
		hash_destruir(hash);
		return NULL;
	}
	clinica->abb_docs = arbol_doctores;
	clinica->abb_pacs = arbol_pacientes;
	clinica->tabla_esps = hash;
	return clinica;
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
			atender_siguiente(parametros, clinica);
	} else if (strcmp(comando, COMANDO_INFORME) == 0) {
		if(cant_params_check(COMANDO_INFORME, CANT_PARAMS_INFORME, parametros))
			informe(parametros, clinica);
	} else {
		printf(ENOENT_CMD, comando);
	}
}


void informe(const char** parametros, clinica_t* clinica){
	int valor_entero_rango = func(parametros[0], parametros[1]);
	iteracion_por_rangos(clinica->abb_docs, funtion, NULL, valor_entero_rango, parametros[0], parametros[1]);
}

void pedir_turno(const char** parametros, clinica_t* clinica){
	cola_pacientes_t* turno_especialidad = hash_obtener(clinica->tabla_esps, parametros[1]);
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
	if(strcmp(parametros[2], PACIENTE_URGENTE) == 0 && ok){
		if(!cola_encolar(turno_especialidad->urgentes, turno_paciente))
			ok = false;
	} else if (strcmp(parametros[2], PACIENTE_REGULAR) == 0 && ok){
		if(!heap_encolar(turno_especialidad->regulares, turno_paciente))
			ok = false;
	} else if (strcmp(parametros[2], PACIENTE_REGULAR) != 0 && strcmp(parametros[2], PACIENTE_URGENTE) != 0){
		printf(ENOENT_URGENCIA, parametros[2]);
		ok = false;
	}
	if(ok){
		printf(PACIENTE_ENCOLADO, parametros[0]);
		printf(CANT_PACIENTES_ENCOLADOS, turno_especialidad->cantidad_en_espera, parametros[1]);
		turno_especialidad->cantidad_en_espera++;
	}
}


void atender_siguiente(const char** parametros, clinica_t* clinica){
	doctor_t* doctor = abb_obtener(clinica->abb_docs, parametros[0]);
	if(!doctor){
		printf(ENOENT_DOCTOR, parametros[0]);
		return;
	}
	doctor->pac_atendidos++;
	cola_pacientes_t* cola_esp = hash_obtener(clinica->tabla_esps, doctor->esp);
	if(cola_esp->cantidad_en_espera == 0){
		printf(SIN_PACIENTES);
		doctor->estado = LIBRE;
		return;
	}
	paciente_t* paciente = cola_desencolar(cola_esp->urgentes);
	if(!paciente)
		paciente = heap_desencolar(cola_esp->regulares);
	printf(PACIENTE_ATENDIDO, paciente->nombre);
	cola_esp->cantidad_en_espera--;
	printf(CANT_PACIENTES_ENCOLADOS, cola_esp->cantidad_en_espera, doctor->esp);
}


void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}


void procesar_entrada(clinica_t* clinica) {
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
		cmd_load(campos[0], (const char**) parametros, clinica);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
}