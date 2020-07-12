build: heap.c 
	gcc -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -o heap heap.c

run: build
	./heap
memoria: build
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./heap
