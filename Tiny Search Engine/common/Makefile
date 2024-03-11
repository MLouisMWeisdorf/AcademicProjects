# CS50 Spring 2023, Lab 5 (Indexer)
# Makefile for Common library
#   Builds and compiles all these utility functions into a library.
# Louis Weisdorf
# 04/27/2023

# object files, and the target library
OBJS = pagedir.o index.o word.o
LIB = common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc
MAKE = make

# Build $(LIB) by archiving object files
# (you will need to drop in copy of set.c, counters.c, hashtable.c)
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# Dependencies: object files depend on header files
pagedir.o: pagedir.h ../libcs50/webpage.h
index.o: index.h ../libcs50/hashtable.h ../libcs50/counters.h ../libcs50/mem.h
word.o: word.h

.PHONY: clean sourcelist

# list all the sources and docs in this directory.
# (this rule is used only by the Professor in preparing the starter kit)
sourcelist: Makefile *.md *.c *.h
	@echo $^

# clean up after our compilation
clean:
	rm -f core
	rm -f $(LIB) *~ *.o
