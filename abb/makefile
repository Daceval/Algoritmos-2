build: abb.c testing.c pruebas_abb_alumno.c pila.c main_abb.c
	gcc -g -std=c99 -Wall -Wno-conversion -Wno-sign-conversion -Werror -o pruebas *.c
run: build
	./pruebas

memoria: build
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./pruebas
