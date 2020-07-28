#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mensajes.h"
#include "hash.h"
#include "abb.h"
// #include "funciones_tp2.c"
//#include "strutil.h"


// #include "hash.h"
// #include "abb.h"
// #include "lista.h"

// #define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
// #define COMANDO_ATENDER "ATENDER_SIGUIENTE"
// #define COMANDO_INFORME "INFORME"

// #define PACIENTE_REGULAR "REGULAR"
// #define PACIENTE_URGENTE "URGENTE"

typedef struct clinica{
	abb_t* abb_docs;
	abb_t* abb_pacs;
	hash_t* tabla_esps;
}clinica_t;

clinica_t* init(char** argv);

void procesar_entrada(clinica_t* clinica);




int main(int argc, char** argv){
	if(argc != 2){
		printf(ENOENT_CANT_PARAMS);
		return 1;
	}
	clinica_t* clinica = init(argv);
	while(1){
		procesar_entrada(clinica);
	}
}
