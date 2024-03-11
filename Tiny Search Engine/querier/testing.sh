#!/bin/bash
# testing.sh
# Author: Louis Weisdorf
# Date: 05/11/2023
# Description: Shell script to test valid and invalid cases 
#       for use of querier.c program.

### Erroneous arguments ###
# No arguments
./querier

# One argument
./querier ../../shared/tse/crawldata/toscrape-1

# Three (or more) arguments
./querier ../../shared/tse/crawldata/toscrape-1 ../../shared/tse/indices/toscrape-1.ndx ../../shared/tse/indices/toscrape-1.ndx

# Non-existent pageDirectory
./querier ../../shared/tse/crawldata/toscrape-4 ../../shared/tse/indices/toscrape-1.ndx

# Non-crawler-produced pageDirectory
./querier ../../shared/tse/indices ../../shared/tse/indices/toscrape-1.ndx

# Unreadable index file
chmod -r ../test-data/letters.index
./querier ../../shared/tse/crawldata/toscrape-1 ../test-data/letters.index


### Runs on querier to test edge cases ###

./querier ../../shared/tse/crawldata/toscrape-1 ../../shared/tse/indices/toscrape-1.ndx <<EOF
and
or
and earth
or earth
planet earth or
planet earth and
planet earth and or science
planet earth and and science
planet earth or and science
Warning!
(The Lunar Chronicles #4.5)
EOF

### Runs on querier with fuzzquery from different seeds ###

# On wikipedia-2, 10 queries
./fuzzquery ../../shared/tse/indices/wikipedia_2.ndx 10 1

./fuzzquery ../../shared/tse/indices/wikipedia_2.ndx 10 1 | ./querier ../../shared/tse/crawldata/wikipedia_2 ../../shared/tse/indices/wikipedia_2.ndx

# On toscrape-1, 10 queries
./fuzzquery ../../shared/tse/indices/toscrape-1.ndx 10 1

./fuzzquery ../../shared/tse/indices/toscrape-1.ndx 10 1 | ./querier ../../shared/tse/crawldata/toscrape-1 ../../shared/tse/indices/toscrape-1.ndx