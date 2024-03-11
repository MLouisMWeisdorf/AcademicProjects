/** 
 * indextest.c
 * Author: Louis Weisdorf
 * CS50 Spring 2023, Lab 5
 * Date: 05/02/2023
 * Description: This program is a tester for the indexer.c program.
 *          Specifically, it takes an index file, reads the content,
 *          and copies it over into a new index file. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../common/index.h"

int main(const int argc, char *argv[]) 
{
    // If the number of arguments given is invalid
    if (argc != 3) {
        // Printing error message and exiting with non-zero
        fprintf(stderr, "\nError: Invalid number of arguments passed.\n");
        exit(1);
    }

    // Saving arguments in variables
    char* oldIndexFilename = argv[1];
    char* newIndexFilename = argv[2];

    // Creating a new index to read the file contents into (and later write)
    index_t* index = index_new(800);

    // Calling the function for reading an index from index file
    index_file_read(index, oldIndexFilename);

    // Calling the function for writing an index into a new file
    index_write(index, newIndexFilename);
}
