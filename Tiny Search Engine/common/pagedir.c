/** 
 * pagedir.c
 * Author: Louis Weisdorf
 * CS50 Spring 2023, TSE
 * Date: 04/24/2023
 * Description: This program handles the pagesaver, marking it as a
 *      crawler-produced pageDirectory. For the Indexer program,
 *      it validates a pageDirectory as being produced by the crawler
 *      and loads the page contents from a crawler file.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../libcs50/webpage.h"
#include "../libcs50/mem.h"
#include "../libcs50/file.h"

/******************** pagedir_init *******************************/
/* See detailed description in pagedir.h. */
bool pagedir_init(const char* pageDirectory) 
{
    // Constructing the pathname for the .crawler file in that directory
    char* pathName = mem_malloc(strlen(pageDirectory) + strlen("/.crawler") + 2);

    // Checking for NULL pointer
    if (pathName == NULL) {
        // Printing error message and exiting with non-zero
        fprintf(stderr, "\nError: The memory allocation was unsuccessful.\n");
        exit(4);
    }

    // Copying contents into allocated memory
    strcpy(pathName, pageDirectory);
    strcat(pathName, "/.crawler");

    // Opening the file for writing
    FILE* file = fopen(pathName, "w");
    
    // If the created file is NULL, returning false
    if (file == NULL) {
        return false;
    }

    // Closing the file
    fclose(file);

    // Freeing memmory allocated for pathName
    mem_free(pathName);

    // Returning true if no errors occurred
    return true;
}

/******************** pagedir_save *******************************/
/* See detailed description in pagedir.h. */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID)
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

    // Opening the file for writing
    FILE* file = fopen(pathName, "w");
    
    // Checking for NULL pointer
    if (file == NULL) {
        // Printing error message and exiting with non-zero
        fprintf(stderr, "\nError: The memory allocation was unsuccessful.\n");
        exit(4);
    }

    // Printing webpage information (URL, depth, contents)
    fprintf(file, "%s\n", webpage_getURL(page));
    fprintf(file, "%d\n", webpage_getDepth(page));
    fprintf(file, "%s\n", webpage_getHTML(page));

    // Closing the file
    fclose(file);

    // Freeing memmory allocated
    mem_free(IDString);
    mem_free(pathName);
}

/******************** pagedir_validate *******************************/
/* See detailed description in pagedir.h. */
bool pagedir_validate(char* pageDirectory)
{
    // Putting together the pathname for the .crawler file in that directory
    char* pathName = mem_malloc(strlen(pageDirectory) + strlen("/.crawler") + 2);

    // Checking for NULL pointer
    if (pathName == NULL) {
        // Printing error message and exiting with non-zero
        fprintf(stderr, "\nError: The memory allocation was unsuccessful.\n");
        exit(4);
    }

    // Copying contents into allocated memory
    strcpy(pathName, pageDirectory);
    strcat(pathName, "/.crawler");

    // Opening the file for reading
    FILE* file = fopen(pathName, "r");
    
    // If the file is NULL, returning false
    if (file == NULL) {
        return false;
    }

    // Closing the file
    fclose(file);

    // Freeing memmory allocated for pathName
    mem_free(pathName);

    // Returning true if no errors occurred
    return true;
}

/******************** pagedir_validate *******************************/
/* See detailed description in pagedir.h. */
webpage_t* pagedir_load(FILE* file) 
{
    // Reading the webpage URL from the first line of the file
    char* webpageURL = file_readLine(file);

    // Reading depth from the second line of the file
    char* depthStr = file_readLine(file);

    // Reading the HTML from the third line on
    char* HTML = file_readFile(file);

    // Checking if the retrieval of URL and depth were successful
    if (webpageURL == NULL || depthStr == NULL || HTML == NULL) {
        // Printing error message and exiting with non-zero
        fprintf(stderr, "\nError: Webpage information retrieval "
            "unsuccessful.\n");
        exit(5);
    }

    // If above check was successful, converting depthStr to int
    int depth = atoi(depthStr);

    // Freeing the memory allocated for the initial string
    mem_free(depthStr);

    // Creating a new webpage object to pass to indexPage
    webpage_t* webpage = webpage_new(webpageURL, depth, HTML);

    // Checking for NULL pointer
    if (webpage == NULL) {
        // Printing error message and exiting with non-zero
        fprintf(stderr, "\nError: The memory allocation was "
            "unsuccessful.\n");
        exit(4);
    }

    // Returning the webpage
    return webpage;
}
