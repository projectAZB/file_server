CC = gcc
CFLAGS = -pedantic -std=gnu99 -Wall
DEBUG_FLAGS = -g $(CFLAGS)
VALGRIND_FLAGS = valgrind --show-reachable=yes --leak-check=full

all: server libmfs

libmfs: mfs.h mfs.c inode_table.h inode.h data_block_table.h data_block.h constants.h
	$(CC) $(DEBUG_FLAGS) -shared -o libmfs.so -fPIC mfs.c

server: libmfs udp.c udp.h
	$(CC) $(DEBUG_FLAGS) server.c udp.c mfs.c -o server

main: main.c libmfs
	$(CC) $(DEBUG_FLAGS) main.c -o server -L./ -llibmfs

.PHONY: clean
clean:
	rm -rf *.o *.dSYM libmfs.so server
