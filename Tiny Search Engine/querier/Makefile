# Makefile for the 'querier' module
# CS50 Lab 6
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

querier: querier.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

querier.o: $L/file.h $L/mem.h $C/word.h $C/index.h $C/pagedir.h

fuzzquery: fuzzquery.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@
fuzzquery.o: $L/file.h $L/mem.h

all: querier fuzzquery

test: testing.sh
	bash -v testing.sh >& testing.out

valgrind: valgrind_test.sh
	bash -v valgrind_test.sh >& valgrind_test.out

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f indexer
	rm -f core