# Makefile for the 'indexer' module
# CS50 Lab 5
# Louis Weisdorf

C = ../common
L = ../libcs50
LIBS =
LLIBS = $C/common.a $L/libcs50-given.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$L -I$C
CC = gcc
MAKE = make

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: test valgrind clean all

all: indexer indextest

indexer: indexer.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indexer.o: $L/webpage.h $L/file.h $L/mem.h $C/word.h $C/index.h $C/pagedir.h

indextest: indextest.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indextest.o: $C/index.h


test: testing.sh
	bash -v testing.sh >& testing.out

valgrind: valgrind_test.sh
	bash -v valgrind_test.sh >& valgrind_test.out

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f indexer
	rm -f core