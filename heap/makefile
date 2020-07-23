build: heap.c testing.c main.c pruebas_alumno.c
	gcc -g -O2 -std=c99 -Wall -Werror -pedantic -Wformat=2 -Wshadow -Wpointer-arith -Wunreachable-code -Wconversion -Wno-sign-conversion -Wbad-function-cast -o pruebas main.c testing.c heap.c pruebas_alumno.c

run: build
	./pruebas
memoria: build
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 ./pruebas
