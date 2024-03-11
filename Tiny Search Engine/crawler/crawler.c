/** 
 * crawler.c
 * Author: Louis Weisdorf
 * CS50 Spring 2023, Lab 4
 * Date: 04/24/2023
 * Description: This program provides the first component of the 
 *      tiny search engine (TSE) project, scanning webpages for URLs.
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "mem.h"
#include "webpage.h"
#include "bag.h"
#include "../common/pagedir.h"
#include "hashtable.h"

// Declaring the functions prior to use
static void parseArgs(const int argc, char* argv[],char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);


/******************** parseArgs *******************************/
/* See detailed description in crawler.h */
static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth)
{
    // If the number of arguments given is invalid
    if (argc != 4) {
        // Printing error message and exiting with non-zero
        fprintf(stderr, "\nError: Invalid number of arguments passed.\n");
        exit(1);
    }
    
    // Checking the seedURL provided
    char* normalizedURL = normalizeURL(*seedURL); // Normalize the seedURL

    // Checking for NULL pointer
    if (normalizedURL == NULL) {
        // Printing error message and exiting with non-zero
        fprintf(stderr, "\nError: The memory allocation was unsuccessful.\n");
        exit(4);
    }

    // If the URL is not an internal URL
    if (!isInternalURL(normalizedURL)) {
        // Printing error message and exiting with non-zero
        fprintf(stderr, "\nError: The URL provided is not an internal URL.\n");
        exit(2);
    }

    // Freeing the memory allocated for normalizedURL
    mem_free(normalizedURL);

    // Checking the maxDepth provided
    if (*maxDepth < 0 || *maxDepth > 10) {
        // Printing error message and exiting with non-zero
        fprintf(stderr, "\nError: The given maxDepth argument is invalid.\n");
        exit(3);
    }

    // Calling pagedir_init() for the pageDirectory
    pagedir_init(*pageDirectory);
}


/******************** crawl *******************************/
/* See detailed description in crawler.h */
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth)
{
    // Creating variable depth to track progress
    int depth = 0;
    int docID = 1;

    // Initializing the hashtable, adding the seedURL
    hashtable_t* hashtable = hashtable_new(200);
    hashtable_insert(hashtable, seedURL, "");

    // Initializing the bag, adding the seedURL at depth 0
    bag_t* bag = bag_new();
    webpage_t* page = webpage_new(seedURL, depth, NULL);
    bag_insert(bag, page);    

    // While the bag is not empty, extracting a webpage
    while ((page = bag_extract(bag)) != NULL) {
        // Fetching the HTML
        if (webpage_fetch(page)) {
            // Printing to stdout for status
            fprintf(stdout, "%d\tFetched: %s\n", webpage_getDepth(page), webpage_getURL(page));

            // Saving the webpage to pageDirectory
            pagedir_save(page, pageDirectory, docID);

            // Incrementing the docID
            docID++;

            // If the webpage is not at maxDepth
            if (depth < maxDepth) {
                // Printing status information to stdout
                fprintf(stdout, "%d\tScanning: %s\n", webpage_getDepth(page), webpage_getURL(page));

                // Calling page scan
                pageScan(page, bag, hashtable);
            }
        }
        // Incrementing the depth
        depth++;

        // Deleting the webpage
        webpage_delete(page);
    }

    // Deleting the hashtable
    hashtable_delete(hashtable, NULL);

    // Deleting the bag
    bag_delete(bag, NULL);
}


/******************** pageScan *******************************/
/* See detailed description in crawler.h */
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen)
{
    // Saving variables for position in the webpage and URL found
    int position = 0;
    char* foundURL;
 
    // While there is still another URL in the page
    while ((foundURL = webpage_getNextURL(page, &position)) != NULL) {
        // Normalizing the URL
        char* normalizedURL = normalizeURL(foundURL);

        // Checking for NULL pointer
        if (normalizedURL == NULL) {
            // Printing error message and exiting with non-zero
            fprintf(stderr, "\nError: The memory allocation was unsuccessful.\n");
            exit(4);
        }

        // Printing status message to stdout
        fprintf(stdout, "%d\tFound: %s\n", webpage_getDepth(page), normalizedURL);

        // If the URL is internal
        if (isInternalURL(normalizedURL)) {
            // Inserting the URL into the hashtable
            if (hashtable_insert(pagesSeen, normalizedURL, "")) {
                // Creating a webpage for the URL, inserting it into the bag
                webpage_t* foundPage = webpage_new(normalizedURL, 
                    webpage_getDepth(page) + 1, NULL);
                bag_insert(pagesToCrawl, foundPage);

                // Printing status message to stdout
                fprintf(stdout, "%d\tAdded: %s\n", webpage_getDepth(page), normalizedURL);
            } 

            // If the webpage is already in the hashtable
            else {
                // Printing status message to stdout
                fprintf(stdout, "%d\tIgnDupl: %s\n", webpage_getDepth(page), normalizedURL);
            }
        }

        // If the webpage is external
        else {
            // Printing status message to stdout
            fprintf(stdout, "%d\tIgnExtrn: %s\n", webpage_getDepth(page), normalizedURL);
        }

        // Freeing the URL
        mem_free(foundURL);
    }
}


int main(const int argc, char *argv[]) 
{
    // Saving variables from the arguments passed to command line
    char* pageDirectory = argv[2];
    
    // Checking if the argv[3] is an integer before passing it along
    int maxDepth;

    // If the maxDepth parameter is not a digit
    if (sscanf(argv[3], "%d", &maxDepth) != 1) {
        // Printing error message and exiting with non-zero
        fprintf(stderr, "\nError: The given maxDepth argument is invalid.\n");
        exit(3);
    }

    // Allocating memory for seedURL
    int stringLength = strlen(argv[1]);
    char* seedURL = (char*) calloc(stringLength + 1, sizeof(char));

    // Checking for NULL pointer
    if (seedURL == NULL) {
        // Printing error message and exiting with non-zero
        fprintf(stderr, "\nError: The memory allocation was unsuccessful.\n");
        exit(4);
    }

    // Copying contents into allocated memory
    strncpy(seedURL, argv[1], stringLength);

    // Calling parseArgs
    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);

    // Calling crawl
    crawl(seedURL, pageDirectory, maxDepth);

    // Returning with exit code 0 for successful termination
    exit(0);
}
