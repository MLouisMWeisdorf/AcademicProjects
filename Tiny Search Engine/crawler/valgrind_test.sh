#!/bin/bash
# valgrind_test.sh
# Author: Louis Weisdorf
# Date: 04/27/2023
# Description: Shell script to test valgrind
#       for use of crawler.c program.

# Calling valgrind on a test call of the crawler function
valgrind ./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ../data/wikipedia 0
