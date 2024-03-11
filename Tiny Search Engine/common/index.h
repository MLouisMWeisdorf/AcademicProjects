/** 
 * index.h
 * Author: Louis Weisdorf
 * CS50 Spring 2023, Lab 5
 * Date: 04/29/2023
 * Description: This is a header file for the index.c program.
 */
#include <stdio.h>
#include "../libcs50/counters.h"

/**************** global types ****************/
typedef struct index index_t;

/**************** functions ****************/

/******************** index_new *******************************/
/* Function creates a new index, returning a pointer to it 
*  if successful, NULL if not.
*  The hashtable in the index is initialized with the parameter
*  slots, provided by the caller.
*  The caller is responsible for later calling index_delete.
*/
index_t* index_new(int slots);

/******************** index_insert *******************************/
/* Function takes an index, a word, and a docID as parameters.
*  If the hashtable belonging to the index does not already 
*  contain the word, the word is added to the hashtable, with a
*  new counter as its item. The counter takes the docID as key and
*  an initial count of 1.
*  If the word is already in the hashtable, we add to the counter
*  for the corresponding docID (or intialize to 1).
*  We return true upon successful insertion, false if errors occur
*  underway or if the index passed is NULL.
*/
bool index_insert(index_t* index, char* word, int docID);

/******************** index_find *******************************/
/* Function finds and returns the counter object associated to
*  a word in the index. The caller provides an index and a word
*  key.  
*/
counters_t* index_find(index_t* index, char* word);

/******************** index_filecheck *******************************/
/* Function takes an indexFilename as parameter. If this
*  file can be successfully opened for writing, it returns true
*  If not, it returns false.
*  The function is called in the main of indexer.c to parse arguments
*  (prior to calling further functions) to save wasted work caused
*  by an invalid indexFilename passed to the commandline.
*/
bool index_filecheck(char* indexFilename);

/******************** index_pathname *******************************/
/* Function takes a pageDirectory and docID, creating a pathname
*  used by indexBuild to access files from the pageDirectory.
*  It returns a string of the generated pathname (or NULL if any
*  errors occur).
*/
char* index_pathname(char* pageDirectory, int docID);

/******************** index_file_read *******************************/
/* Function takes an index and an oldIndexFilename as parameters.
*  It is used in the indextest.c to read an index file into an index.
*  If the provided file cannot be opened, it exits with non-zero
*  status. The caller is responsible for later freeing the allocated
*  memory for the index passed to the function (which occurs in
*  index_write, which should be called after the function).
*/
void index_file_read(index_t* index, char* oldIndexFilename);

/******************** index_write *******************************/
/* Function takes an index and the filename intended to write
*  the index into. After checking for NULL index or filename,
*  the function opens the index file, then calling on
*  the set_iterate_helper and then counters_iterate_helper.
*  Together, these functions iterate over the contents of the index,
*  writing it into file in the format 
*  "word docID count [docID count]...". 
*/
void index_write(index_t* index, char* indexFilename); 

/******************** set_iterate_helper *******************************/
/* Function is a helper function for the index_write function,
*  supplying the "itemfunc" in the hashtable_iterate
*  (and, by extension, set_iterate) function. It takes the opened file,
*  word, and a counters struct as parameters. The helper function
*  writes a word into the index file and calls the
*  counter_iterate function.
*/
void set_iterate_helper(void* arg, const char* word, void* item);

/******************** counters_iterate_helper *******************************/
/* Function is a helper function for the for the index_write function, through
*  the set_iterate_helper function. It takes the file created by 
*  set_iterate_helper, docID, and corresponding word count as parameters.
*  The function prints the docID and word count into the file passed from
*  set_iterate_helper.
*/
void counters_iterate_helper(void* arg, const int docID, const int count);

/******************** index_delete *******************************/
/* Function deletes an index, freeing up the memory allocated in
*  index_new() and calls hashtable_delete() on the hashtable stored
*  in the index. If the index is null, the function does not do
*  anything.
*/
void index_delete(index_t* index);

/******************** counters_delete_helper *******************************/
/* Function acts as a helper function for the index_delete function,
*  responsible for deleting the counters in the index. It takes a void item,
*  calling the counters_delete function on the item.   
*/
void counters_delete_helper(void* item);