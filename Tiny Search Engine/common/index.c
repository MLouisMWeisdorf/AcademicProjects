/** 
 * index.c
 * Author: Louis Weisdorf
 * CS50 Spring 2023, Lab 5
 * Date: 04/29/2023
 * Description: This program provides the data structure
 *      to represent the in-memory index, as well as functions
 *      to read and write index files. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/mem.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/file.h"

/**************** global types ****************/
// Creating a data type for the index
typedef struct index {
    hashtable_t* hashtable;
} index_t;

/**************** functions ****************/
index_t* index_new(int slots);
bool index_insert(index_t* index, char* word, int docID);
counters_t* index_find(index_t* index, char* word);
bool index_filecheck(char* indexFilename);
char* indexPathname(char* pageDirectory, int docID);
void index_file_read(index_t* index, char* oldIndexFilename);
void index_write(index_t* index, char* indexFilename);
void set_iterate_helper(void* arg, const char* word, void* item);
void counters_iterate_helper(void* arg, const int docID, const int count);
void index_delete(index_t* index);
void counters_delete_helper(void* item);


/******************** index_new *******************************/
/* See detailed description in index.h. */
index_t* index_new(int slots)
{
    // Allocating memory for index
    index_t* index = mem_malloc(sizeof(index_t));

    // Checking if memory allocation was successful, returning NULL if not
    if (index == NULL) {
        return NULL;
    }

    // Initializing the hashtable in the index
    index->hashtable = hashtable_new(slots);
    
    // Returning a pointer to the created index
    return index;
}

/******************** index_insert *******************************/
/* See detailed description in index.h. */
bool index_insert(index_t* index, char* word, int docID) 
{
    // Checking if the index is NULL, returning false if so
    if (index == NULL) {
        return false;
    }

    // Initializing a counter object
    counters_t* counter = hashtable_find(index->hashtable, word);

    // If there is not already a key in the counter for word
    if (counter == NULL) {
        // Creating a counter to store the docID,count pairs for the key
        counter = counters_new();

        // Inserting the key and corresponding counter into hashtable
        hashtable_insert(index->hashtable, word, counter);
    }
    
    // Adding the docID to the counter
    counters_add(counter, docID);

    // Returning true upon successful insertion or addition
    return true;
}

/******************** index_find *******************************/
/* See detailed description in index.h. */
counters_t* index_find(index_t* index, char* word)
{
    // Saving the result of hashtable find in counters object, returning
    counters_t* counters = hashtable_find(index->hashtable, word);
    return counters;
}

/******************** index_filecheck *******************************/
/* See detailed description in index.h. */
bool index_filecheck(char* indexFilename)
{
    // Opening the file for writing
    FILE* file = fopen(indexFilename, "w");
    
    // If the file is NULL, returning false
    if (file == NULL) {
        return false;
    }

    // Closing the file
    fclose(file);

    // Returning true if file was successfully opened for writing
    return true;
}

/******************** indexPathname *******************************/
/* See further details in index.h. */
char* index_pathname(char* pageDirectory, int docID) 
{
    // Constructing the pathname for the file in that directory
    int length = snprintf(NULL, 0, "%d", docID); // Calculating length of ID
    char* IDString = mem_malloc(length + 1); // Allocating memory

    // Checking for NULL pointer
    if (IDString == NULL) {
        // Printing error message and exiting with non-zero
        fprintf(stderr, "\nError: The memory allocation was unsuccessful.\n");
        exit(4);
    }

    sprintf(IDString, "%d", docID); // Saving docID as string in IDString

    // Allocating memory for the pathName
    char* pathName = mem_malloc(strlen(pageDirectory) + strlen(IDString) + 2);

    // Checking for NULL pointer
    if (pathName == NULL) {
        // Printing error message and exiting with non-zero
        fprintf(stderr, "\nError: The memory allocation was unsuccessful.\n");
        exit(4);
    }

    // Copying contents into allocated memory
    strcpy(pathName, pageDirectory);
    strcat(pathName, "/");
    strcat(pathName, IDString);

    // Freeing memory allocated
    mem_free(IDString);
    
    // Returning the generated pathName
    return pathName;
}

/******************** index_file_read *******************************/
/* See detailed description in index.h. */
void index_file_read(index_t* index, char* oldIndexFilename) 
{
    // Opening the file
    FILE* file = fopen(oldIndexFilename, "r");

    // Checking outcome of opening
    if (file == NULL) {
        // Printing error message and exiting with non-zero
        fprintf(stderr, "\nError: Unable to open oldIndexFilename file.\n");
        exit(6);
    }

    // Initializing variable to store line
    char* line;

    // Setting the delimiter
    char* delimiter = " \t\n";

    // Looping over lines in the file
    while ((line = file_readLine(file)) != NULL) {
        // Reading the first word
        char* word = strtok(line, delimiter);

        // Initializing a counter object
        counters_t* counter = counters_new();

        /* Initializing position to 1, used to track our
        position in line (after the word) */
        int position = 1;

        // Initializing variable to hold current word (number)
        char* current = word;

        // Initializing variables to hold docID and count
        int docID;
        int count;
        
        // Looping over remaining numbers in the line
        while ((current = strtok(NULL, delimiter)) != NULL) {
            // If the position is an odd number (it represents a docID)
            if (position % 2 != 0) {
                // Saving current as an integer docID
                docID = atoi(current);
            }

            // If the position is an even number (it represents a count)
            else {
                // Saving current as an integer count
                count = atoi(current);

                // Calling counters_set with docID and count
                counters_set(counter, docID, count);
            }

            // Incrementing the position
            position++;
        }

        // Inserting the word and corresponding counter into index
        hashtable_insert(index->hashtable, word, counter);

        // Freeing memory for the line read into
        mem_free(line);
    }

    // Closing the file
    fclose(file);
}

/******************** index_write *******************************/
/* See detailed description in index.h. */
void index_write(index_t* index, char* indexFilename) 
{
    // If the indexFilename is not valid for writing into
    if (!index_filecheck(indexFilename)) {
        // Printing error message and exiting with non-zero
        fprintf(stderr, "\nError: Provided indexFilename is invalid.\n");
        exit(3);
    } 

    // Opening the file for writing
    FILE* file = fopen(indexFilename, "w");
    
    /* Calling the hashtable iterator on the hashtable in the index struct,
    passing in the iterator helper function for the set */
    hashtable_iterate(index->hashtable, file, set_iterate_helper);

    // Closing the file after iterating is done
    fclose(file);
    
    // Deleting the index passed to the function
    index_delete(index);
}

/******************** set_iterate_helper *******************************/
/* See detailed description in index.h. */
void set_iterate_helper(void* arg, const char* word, void* item)
{
    // Casting the void arg to a file
    FILE* file = (FILE*)arg;

    // Casting the void item to a counter
    counters_t* counters = (counters_t*)item;

    // Opening the file, writing the word into it
    fprintf(file, "%s ", word);

    // Calling the counters iterator 
    counters_iterate(counters, file, counters_iterate_helper);

    // Printing a newline for the next word in the index
    fprintf(file, "\n");
}

/******************** counters_iterate_helper *******************************/
/* See detailed description in index.h. */
void counters_iterate_helper(void* arg, const int docID, const int count)
{
    // Casting the void arg to a file
    FILE* file = (FILE*)arg;
    
    // Writing the docID and corresponding word count into the file
    fprintf(file, "%d %d ", docID, count);
}

/******************** index_delete *******************************/
/* See detailed description in index.h. */
void index_delete(index_t* index)
{
    // Checking if the index passed to the function is null
    if (index != NULL) {
        // Deleting the hashtable belonging to the index
        hashtable_delete(index->hashtable, counters_delete_helper);

        // Freeing the memory allocated for the index
        mem_free(index);
    }
}

/******************** counters_delete_helper *******************************/
/* See detailed description in index.h. */
void counters_delete_helper(void* item) 
{
    // Calling counters delete on the item
    counters_delete(item);
}
