#!/bin/bash
# testing.sh
# Author: Louis Weisdorf
# Date: 04/27/2023
# Description: Shell script to test valid and invalid cases 
#       for use of crawler.c program.

### Erroneous arguments ###
# Invalid number of arguments
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 1 2

# External URL provided
./crawler https://en.wikipedia.org/wiki/Algorithm/ ../data/letters 0

# Invalid maxDepth arguments
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 12

./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters a

### Runs over all three CS50 websites ###
# Running letters (depths 0,4)
mkdir -p ../data/letters/letters-1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters/letters-1 0

mkdir -p ../data/letters/letters-2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters/letters-2 4

## Running toscrape (depths 0,1)
mkdir -p ../data/toscrape/toscrape-1
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ ../data/toscrape/toscrape-1 0

mkdir -p ../data/toscrape/toscrape-2
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ ../data/toscrape/toscrape-2 1

## Running wikipedia (depths 0,1)
mkdir -p ../data/wikipedia/wikipedia-1
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ../data/wikipedia/wikipedia-1 0

mkdir -p ../data/wikipedia/wikipedia-2
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ../data/wikipedia/wikipedia-2 1
