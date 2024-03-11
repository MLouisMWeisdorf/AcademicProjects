#!/bin/bash
# valgrind_test.sh
# Author: Louis Weisdorf
# Date: 05/11/2023
# Description: Shell script to run valgrind on the 
#          querier.c program.

# Calling valgrind on a test call of the querier with wikipedia-2
valgrind ./fuzzquery ../../shared/tse/indices/wikipedia_2.ndx 10 1 | ./querier ../../shared/tse/crawldata/wikipedia_2 ../../shared/tse/indices/wikipedia_2.ndx

# Calling valgrind on a test call of the querier with toscrape-1
valgrind ./fuzzquery ../../shared/tse/indices/toscrape-1.ndx 10 1 | ./querier ../../shared/tse/crawldata/toscrape-1 ../../shared/tse/indices/toscrape-1.ndx