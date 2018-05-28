CC = gcc
CFLAGS = -pedantic -std=gnu99 -Wall
DEBUG_FLAGS = -g $(CFLAGS)
VALGRIND_FLAGS = valgrind --show-reachable=yes --leak-check=full

server: main.c constants.h
	$(CC) $(DEBUG_FLAGS) main.c -o server

.PHONY: clean
clean:
	rm -rf *.dSYM server
