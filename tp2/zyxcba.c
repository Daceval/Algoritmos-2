#include <stdio.h>
#include <string.h>
#include "funciones_tp2.h"
#include "strutil.h"
#include "mensajes.h"

#include "hash.h"
#include "abb.h"
#include "lista.h"

#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"

#define PACIENTE_REGULAR "REGULAR"
#define PACIENTE_URGENTE "URGENTE"


int comparar_pacientes(const void* a, const void* b){
	datos_paciente_t* dato1 = (datos_paciente_t*)a;
	datos_paciente_t* dato2 = (datos_paciente_t*)b;
	if (dato1->antiguedad <  dato2->antiguedad){
		return 1;
	}
	else if (dato1->antiguedad > dato2->antiguedad){
		return -1; 
	}else{
		return 0;
	}
}


bool validar_pedir_turno(hash_t* especialidades, abb_t* pacientes){
	bool es_entrada_valida = true;

	if (!abb_pertenece(pacientes, parametros[0])){
		printf(ENOENT_PACIENTE, parametros[0]);
		entrada_valida = false;
	}
	if (!hash_pertenece(especialidades, parametros[1])){
		printf(ENOENT_ESPECIALIDAD, parametros[1]);
		entrada_valida = false;
	}
	if (strmcp(parametros[2], PACIENTE_REGULAR) != 0 || strmcp(parametros[2], PACIENTE_URGENTE) != 0){
		printf(ENOENT_URGENCIA, parametros[2]);
		entrada_valida = false;
	}

	return es_entrada_valida;

}


void procesar_comando(const char* comando, const char** parametros, hash_t* hash, abb_t* abb1, abb_t* abb2) {

	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {

		if (validar_pedir_turno(hash, abb1)){
			pedir_turno(parametros, hash, abb2);
		}

	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {



	} else if (strcmp(comando, COMANDO_INFORME) == 0) {



	} else {



	}
}


void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}


void procesar_entrada(hash_t* sala_de_espera, abb_t* doctores, abb_t* pacientes) {
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
		procesar_comando(campos[0], parametros, sala_de_espera, doctores, pacientes);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
}


void inicializar_base_datos_pacientes(abb_t* abb, lista_t* lista){
	
	lista_iter_t* iter = lista_iter_crear(lista);
	if(!iter) return;

	while (!lista_iter_al_final(iter)){
		campos_t* informacion = iter_ver_actual(iter);
		const char* nombre_paciente = obtener_nombre(informacion);
		const char* dato = obtener_dato(informacion);
		
		char* nombre = strdup(nombre_paciente);
		char* datos_paciente = strdup(dato);

		abb_guardar(nombre, datos_paciente);
		lista_iter_avanzar(iter);
	}
	iter_lista_destruir(iter);

}

void inicializar_base_datos_doctores(abb_t* abb, lista_t* lista, hash_t* hash){
	lista_iter_t* iter = lista_iter_crear(lista);
	if(!iter) return;

	while (!lista_iter_al_final(iter)){
		pacientes_t* lista_pacientes = pacientes_crear(comparar_pacientes);
		if (!lista_pacientes){
			iter_lista_destruir(iter);
			return;
		}
		campos_t* informacion = iter_ver_actual(iter);
		const char* nombre_doctor = obtener_nombre(informacion);
		const char* especialidad_doctor = obtener_dato(informacion);
		
		char* nombre = strdup(nombre_doctor);
		char* especialidad = strdup(especialidad_doctor);
		hash_guardar(especialidad, lista_pacientes);

		abb_guardar(nombre, especialidad);
		lista_iter_avanzar(iter);
	}
	iter_lista_destruir(iter);
}


int main(int argc, char* argv[]) {
	
	lista_t* lista1 = csv_crear_estructura(argv[1], parseo_datos, NULL);
	lista_t* lista2 = csv_crear_estructura(argv[2], parseo_datos, NULL);
	
	abb_t* base_datos_doctores = abb_crear(strcmp, NULL);

	abb_t* base_datos_pacientes = abb_crear(strcmp, NULL);

	hash_t* sala_de_espera = hash_crear(NULL);

	/* cargo los datos del csv al los abb */
	inicializar_base_datos_pacientes(base_datos_doctores, lista1);
	inicializar_base_datos_doctores(base_datos_pacientes, lista2, sala_de_espera);
	
	/* destruyo las lista con los datos del csv */
	lista_destruir(lista1);
	lista_destruir(lista2);

	procesar_entrada(sala_de_espera, base_datos_doctores, base_datos_pacientes); //procesamos la linea de entrada del usuario por consola.

	abb_destruir(base_datos_doctores);
	abb_destruir(base_datos_pacientes);
	hash_destruir(sala_de_espera)

	return 0;
}
