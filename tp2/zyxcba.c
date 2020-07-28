#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "funciones_tp2.c"
//#include "strutil.h"
#include "mensajes.h"

// #include "hash.h"
// #include "abb.h"
// #include "lista.h"

// #define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
// #define COMANDO_ATENDER "ATENDER_SIGUIENTE"
// #define COMANDO_INFORME "INFORME"

// #define PACIENTE_REGULAR "REGULAR"
// #define PACIENTE_URGENTE "URGENTE"



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
