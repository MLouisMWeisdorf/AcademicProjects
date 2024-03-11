#!/bin/bash
# valgrind_test.sh
# Author: Louis Weisdorf
# Date: 05/03/2023
# Description: Shell script to run valgrind on the 
#          index.c and indextest.c.

# Calling valgrind on a test call of the indexer function
valgrind ./indexer ../../shared/tse/crawldata/letters-3 ../new-test-data/letters.index

# Calling valgrind on a test call of the indextest function
valgrind ./indextest ../new-test-data/letters.index ../new-test-data/new-letters.index