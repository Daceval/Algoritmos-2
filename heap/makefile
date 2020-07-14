build: heap.c testing.c main.c pruebas_alumno.c
	gcc -g -std=c99 -Wall -Wno-conversion -Wtype-limits -pedantic -Werror -o pruebas main.c testing.c heap.c pruebas_alumno.c

run: build
	./pruebas
memoria: build
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./pruebas
