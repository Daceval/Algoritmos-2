CC	= gcc -g -O2 -std=c99 

CFLAGS	= -Wall -Werror -pedantic -Wformat=2 -Wshadow -Wpointer-arith -Wunreachable-code -Wconversion -Wno-sign-conversion -Wbad-function-cast

OBJECT	= -o

HEAPDIR	= ./heap/

ABBDIR	= ./abb/

SOURCEDIR	= ./tp2/


memcheck: build
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 ./pruebas

buildabb: $(ABBDIR)abb.c
	$(CC) $(CFLAGS) $(OBJECT)

build: 
	$(CC) $(CFLAGS) $(OBJECT)
