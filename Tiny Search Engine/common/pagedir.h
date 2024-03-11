/** 
 * pagedir.h
 * Author: Louis Weisdorf
 * CS50 Spring 2023, Lab 4
 * Date: 04/24/2023
 * Description: Header file for the pagedir.c program.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../libcs50/webpage.h"

/******************** pagedir_init *******************************/
/* Function creates a pathname for the .crawler file in the provided
*  directory.
*  Upon error, function returns false.
*  Upon successful completion, function returns true.
*/
bool pagedir_init(const char* pageDirectory);

/******************** pagedir_save *******************************/
/* Function creates a pathname for the webpage passed in,
*  printing the URL, depth, and HTML in the file with that
*  pathname.
*/
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID); 


/******************** pagedir_validate *******************************/
/* Function is used to validate the pageDirectory passed to the 
*  commandline when calling indexer.c.
*  The user passes in the directory: the functions returns true if 
*  the directory is one produced by the crawler, false if not.
*/
bool pagedir_validate(char* pageDirectory);

/******************** pagedir_load *******************************/
/* Function is used to load webpage information from a crawler-
*  produced file into a webpage struct. The function takes a file,
*  reading its contents. If any of the loaded content or the
*  created webpage are NULL, the function exits with non-zero
*  status. Otherwise, the function returns the produced webpage
*  object.
*/
webpage_t* pagedir_load(FILE* file);