/** 
 * crawler.h
 * Author: Louis Weisdorf
 * CS50 Spring 2023, Lab 4
 * Date: 04/25/2023
 * Description: This is a header file for the crawler.c program.
 */
#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"
#include "webpage.h"
#include "pagedir.h"
#include "bag.h"


/******************** parseArgs *******************************/
/* Function checks arguments passed to the command line,
*  eventually calling pagedir_init() if no errors.
*/
static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth);


/******************** crawl *******************************/
/* Function crawls from the seedURL to maxDepth, saving
*  pages in pageDirectory as we go. 
*/
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);


/******************** pageScan *******************************/
/* Function crawls from the seedURL to maxDepth, saving
*  pages in pageDirectory as we go. 
*/
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);
