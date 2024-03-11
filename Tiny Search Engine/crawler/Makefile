# Makefile for the 'crawler' module
# CS50 Lab 4
# Louis Weisdorf

OBJS = crawler.o ../libcs50/webpage.o ../common/pagedir.o ../libcs50/mem.o
LIBS =
LLIBS = ../libcs50//libcs50-given.a

# uncomment the following to turn on verbose memory logging
TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50 -I ../common
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

crawler: $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LLIBS) $(LIBS) -o $@

crawler.o: crawler.h ../libcs50/webpage.h ../common/pagedir.h

.PHONY: test valgrind clean

# expects a file ` to exist; it can contain any text.
test: testing.sh
	bash -v testing.sh >& testing.out

# expects a file ` to exist; it can contain any text.
valgrind: valgrind_test.sh
	bash -v valgrind_test.sh >& valgrind_test.out

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f crawler
	rm -f core