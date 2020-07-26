#include <stdio.h>
#include "csv.h"
#include "ejemplo_struct.h"

bool imp(void* dato, void* extra) {
	ejemplo_imprimir(dato);
	return true;
}

void wrapper_dest(void* dato) {
	ejemplo_destruir(dato);
}

int main(int argc, char** argv) {
	lista_t* ejemplos = csv_crear_estructura(argv[1], parse_ejemplo, NULL);
	lista_iterar(ejemplos, imp, NULL);
	lista_destruir(ejemplos, wrapper_dest);
	return 0;
}

