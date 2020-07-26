#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "ejemplo_struct.h"


struct ejemplo {
	int numerito;
	char* nombrecito;
};

static char* strdup(const char* org) {
	char* cad = malloc(sizeof(char) * (strlen(org) + 1));
	if (!cad) return NULL;
	strcpy(cad, org);
	return cad;
}

ejemplo_t* ejemplo_crear(const char* nombre, int numero) {
	ejemplo_t* ejemplo = malloc(sizeof(ejemplo_t));
	if (!ejemplo) return NULL;
	ejemplo->nombrecito = strdup(nombre);
	if (!ejemplo->nombrecito) {
		free(ejemplo);
		return NULL;
	}
	ejemplo->numerito = numero;
	return ejemplo;
}

void ejemplo_destruir(ejemplo_t* ej) {
	free(ej->nombrecito);
	free(ej);
}

void ejemplo_imprimir(const ejemplo_t* ej) {
	printf("%s - %d\n", ej->nombrecito, ej->numerito);
}

void* parse_ejemplo(char** params, void* extra) {
	return ejemplo_crear(params[0], atoi(params[1]));
}

