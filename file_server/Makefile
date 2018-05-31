CC = gcc
CFLAGS = -pedantic -std=gnu99 -Wall
DEBUG_FLAGS = -g $(CFLAGS)
VALGRIND_FLAGS = valgrind --show-reachable=yes --leak-check=full

server: main.c mfs.h mfs.c inode_table.h inode.h data_block_table.h data_block.h
	$(CC) $(DEBUG_FLAGS) main.c mfs.c -o server

.PHONY: clean
clean:
	rm -rf *.dSYM server
