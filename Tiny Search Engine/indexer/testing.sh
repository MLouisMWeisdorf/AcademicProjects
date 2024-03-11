#!/bin/bash
# testing.sh
# Author: Louis Weisdorf
# Date: 05/03/2023
# Description: Shell script to test valid and invalid cases 
#       for use of indexer.c program.

### Erroneous arguments ###
# No arguments
./indexer

# One argument
./indexer ../../shared/tse/crawldata/letters-3

# Three or more arguments
./indexer ../../shared/tse/crawldata/letters-3 ../test-data/letters1.index ../test-data/letters2.index

# Non-existent pageDirectory
./indexer ../../shared/tse/crawldata/letters-4 ../test-data/letters.index

# Non-crawler-produced pageDirectory
mkdir ../test-data/non-crawler
./indexer ../test-data/non-crawler ../test-data/letters.index

# Read-only directory 
chmod -w ../test-data
./indexer ../test-data/letters ../test-data/letters.index

# Read-only indexFile
chmod +w ../test-data
chmod -w ../test-data/letters.index
./indexer ../../shared/tse/crawldata/letters-3 ../test-data/letters.index

### Runs on various crawler-produced pageDirectories ###

# On letters-3, followed by indexTest run and diff to check similarities
mkdir -p ../new-test-data
./indexer ../../shared/tse/crawldata/letters-3 ../new-test-data/letters.index
./indextest ../new-test-data/letters.index ../new-test-data/new-letters.index
diff ../new-test-data/letters.index ../new-test-data/new-letters.index

# On wikipedia-1
./indexer ../../shared/tse/crawldata/wikipedia_1 ../new-test-data/wikipedia.index

echo