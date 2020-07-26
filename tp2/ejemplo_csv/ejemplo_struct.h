#ifndef _EJEMPLO_
#define _EJEMPLO_

typedef struct ejemplo ejemplo_t;

ejemplo_t* ejemplo_crear(const char* nombre, int numero);

void ejemplo_destruir(ejemplo_t* ej);

void ejemplo_imprimir(const ejemplo_t* ej);

void* parse_ejemplo(char** params, void* extra);

#endif
