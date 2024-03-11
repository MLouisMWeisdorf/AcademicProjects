/** 
 * querier.c
 * Author: Louis Weisdorf
 * CS50 Spring 2023, Lab 6
 * Date: 05/07/2023
 * Description: This program provides the final component of the 
 *      tiny search engine (TSE) project, the querier, allowing searches.
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "../libcs50/mem.h"
#include "../libcs50/file.h"
#include "../common/pagedir.h"
#include "../common/index.h"
#include "../common/word.h"

/**************** types ****************/
// Making a struct to hold docID and score (for ranking purposes)
typedef struct rankElement
{
    int docID;
    int score; 
} rankElement_t;

// Making a struct to store result and copy counter
typedef struct counterStruct
{
    counters_t* result;
    counters_t* copy;
} counterStruct_t;

/**************** functions *******************************/

// Declaring functions used throughout
int fileno(FILE *stream);
static rankElement_t* rankElement_new(int docID, int score);
static void rankElement_delete(rankElement_t* element);
static void prompt(void);
void inputHandler(index_t* index, char* pageDirecory);
int parseLine(char** words, char* line);
bool inputValidate(char** words, int wordCount);
bool operatorCheck(char* word);
counters_t* queryHandler(index_t* index, char** words);
void disjunctionMerge(counters_t* result, counters_t* temp);
void disjunctionMergeHelper(void* arg, const int key, const int count);
counters_t* conjunctionIntersect(counters_t* temp, counters_t* copy);
void conjunctionIntersectHelper(void* arg, const int key, const int count);
counters_t* countersCopy(counters_t* counters);
void countersCopyHelper(void* arg, const int key, const int count);
int countersCount(counters_t* counter);
void countersCountHelper(void* arg, const int key, const int count);
void queryOutput(counters_t* result, char* pageDirectory);
rankElement_t* resultRanking(counters_t* result);
void resultRankingHelper(void* arg, const int key, const int count);

/**************** local functions *******************************/

/******************** rankElement_new *******************************/
/* Function creates a new instance of the rankElement struct.
*  It allocates memory for the struct and sets it docID and count to
*  the parameters passed in.
*  The caller is responsible for later freeing the memory allocated
*  for the struct.
*/
static rankElement_t* rankElement_new(int docID, int score)
{
    // Allocating memory for the new node
    rankElement_t* element = mem_malloc(sizeof(rankElement_t));

    // If an error occurs allocating memory, returning NULL
    if (element == NULL) {
        return NULL;

    // If memory allocation was successful, setting properties, returning
    } else {
        element->docID = docID;
        element->score = score;

        return element;
    }
}


/******************** rankElement_delete *******************************/
/* Function deletes an instance of the rankElement struct by freeing 
*  the struct (if not NULL). The function takes in the struct to be
*  deleted. If the struct is NULL, we do nothing.
*/
static void rankElement_delete(rankElement_t* element)
{
    // Checking if the element is not NULL, freeing the element
    if (element != NULL) {
        mem_free(element);
    }
}


/******************** counterStruct_new *******************************/
/* Function creates a new instance of the counterStruct. The function
*  takes in the two parameters to be stored. After allocating memory
*  the counters are stored in the struct, before a pointer to the
*  created struct is returned. 
*/
static counterStruct_t* counterStruct_new(counters_t* result, counters_t* copy)
{
    // Allocating memory for the new struct
    counterStruct_t* counterStruct = mem_malloc(sizeof(counterStruct_t));

    // If an error occurs allocating memory, returning NULL
    if (counterStruct == NULL) {
        return NULL;

    // If memory allocation was successful, setting properties, returning
    } else {
        counterStruct->result = result;
        counterStruct->copy = copy;

        return counterStruct;
    }
}


/******************** prompt *******************************/
/* Function given in instructions. The function prints
*  the query prompt, only if stdin is a keyboard.
*/
static void prompt(void)
{
  // print a prompt iff stdin is a tty (terminal)
  if (isatty(fileno(stdin))) {
    printf("Query? ");
  }
}


/**************** global functions *******************************/

// Main function
int main(const int argc, char *argv[])
{
    // Checking number of arguments
    if (argc != 3) {
        // Printing error message and exiting with non-zero
        fprintf(stderr, "\nError: Invalid number of arguments passed.\n");
        exit(1);
    }

    // Checking if the first argument is a valid crawler pageDirectory
    if (!pagedir_validate(argv[1])) {
        // Printing error message and exiting with non-zero
        fprintf(stderr, "\nError: Provided pageDirectory is invalid.\n");
        exit(2);
    }

    // Checking if the second argument (indexFilename) is a readable pathname
    FILE* indexFile;
    if ((indexFile = fopen(argv[2], "r")) == NULL) {
        // Printing error message and exiting with non-zero
        fprintf(stderr, "\nError: Provided indexFile is not readable.\n");
        exit(3);
    }

    // Gathering the number of lines in the file (used to initialize index)
    int numLines = file_numLines(indexFile);
    fclose(indexFile);

    // Loading the index from indexFilename into the index data structure
    index_t* index = index_new(numLines);
    index_file_read(index, argv[2]);

    // Calling inputHandler, passing in index and pageDir
    inputHandler(index, argv[1]);

    // Cleaning up
    index_delete(index);

    // Exiting with 0 upon successful termination
    exit(0);
}


/******************** inputHandler *******************************/
/* Function is called from main, takes an index and pageDirectory
*  as parameters. It handles all the functionality revolving 
*  around the input reading and processing (either within function
*  or by calling other functions).
*/
void inputHandler(index_t* index, char* pageDirectory)
{
    // Reading queries from the stdin, one per line, until EOF
    while (!feof(stdin)) {
        // Prompting user input
        prompt();

        // Reading the line
        char* line = file_readLine(stdin);

        // Checking if the line is empty, moving on
        if (line == NULL) {
            continue;
        }

        // Allocate memory for an array of pointers to words
        char** words = mem_calloc(strlen(line) + 1, sizeof(char*));

        // Calling function to parse line into words
        int wordCount = parseLine(words, line);

        // Checking if an error occurred during the parsing
        if (wordCount == 0) {
            // Cleaning up and moving on to next query prompt
            mem_free(line);
            mem_free(words);
            continue;
        }

        // Validating the input
        if (!inputValidate(words, wordCount)) {
            // Cleaning up and moving onto next query prompt
            mem_free(line);
            mem_free(words);
            continue;
        }

        // Calling the queryHandler, saving it in a result counter
        counters_t* result = queryHandler(index, words);

        // Calling the sorter and output function
        queryOutput(result, pageDirectory);
        
        // Cleaning up (individual words, word array, and result counter)
        mem_free(line);
        mem_free(words);
        counters_delete(result);
    }
}


/******************** parseLine *******************************/
/* Function takes in a line read from the stdin, as well as
*  an array to store the words. It parses the input 
*  into single words with a null terminator following, stored
*  in the words array.  
*/
int parseLine(char** words, char* line) 
{
    // Setting a variable to track number of words tokenized
    int wordCount = 0;

    // Setting variables to track position in line and word array, respectively
    int i = 0;
    int j = 0;

    // Setting pointer to the start of the word (first non-space)
    char* wordStart;

    // Setting pointer to track the rest of the word 
    char* wordRest;

    // Setting boolean to determine whether we are working on a new word
    bool newWord = true;

    // Iterating over the query input
    while (line[i] != '\0') {
        // If the current character is neither a space or alphabet (invalid)
        if (!(isspace(line[i])) && !(isalpha(line[i]))) {
            // Printing error message, returning 0
            fprintf(stderr, "Error: invalid character '%c'\n", line[i]);
            return 0;
        }

        // Making the character lowercase
        line[i] = tolower(line[i]);
        
        // If we are working on a new word
        if (newWord) {
            // If the current character is an alphabet (valid)
            if (isalpha(line[i])) {
                // Point start and rest of word to current character
                wordStart = &(line[i]);
                wordRest = &(line[i]);
                newWord = false;
            }
        }

        // If we are working on a word
        else {
            // Pointing wordRest to current character
            wordRest = &(line[i]);

            // If the current character is a space
            if (isspace(*wordRest)) {
                // Set the current character to a null terminator
                *wordRest = '\0';

                // In the words array, save start position of the word
                words[j] = wordStart;

                // Increment index for array and set to newWord
                j++;
                wordCount++;
                newWord = true;
            }
        }

        // Moving onto next character in line
        i++;
    }

    // If we are working on a word not yet added
    if (!newWord) {
        words[j] = wordStart;
        wordCount++;
    }

    // Returning the count
    return wordCount;
}


/******************** inputValidate *******************************/
/* Function takes in a an array of pointers to characters (list of
*  words), and checks if the query is valid. This includes checking: 
*       - if the string starts or ends with and/or
*       - if there are consecutive and/or
*       - if words are shorter than 3 characters (except and/or)
*  The function returns false if any of these apply, true if tests
*  are passed.
*/
bool inputValidate(char** words, int wordCount)
{   
    // Checking for valid parameter
    if (words == NULL) {
        return false;
    }

    // Checking if the string starts with and/or
    if (operatorCheck(words[0])) {
        // Printing error message to stdout and returning false
        fprintf(stderr, "Error: '%s' cannot be first\n", words[0]);
        return false;
    }

    // Checking if the string ends with and/or
    if (operatorCheck(words[wordCount - 1])) {
        // Printing error message to stdout and returning false
        fprintf(stderr, "Error: '%s' cannot be last\n", words[wordCount - 1]);
        return false;
    }

    // Looping through the words
    int i = 0;
    while (i < wordCount) {
        // Checking if the word is shorter than 3 characters (and not operator)
        if ((!(word_check(words[i]))) && (!operatorCheck(words[i]))) {
            // Printing error message to stdout and returning false
            fprintf(stderr, "Error: query words ('%s') cannot be shorter "
                "than 3 chars\n", words[i]);
            return false;
        }

        if (words[i + 1] != NULL) {
            if (operatorCheck(words[i]) && operatorCheck(words[i + 1])) {
            // Printing error message to stdout and returning false
            fprintf(stderr, "Error: '%s' and '%s' cannot be adjacent\n", 
                words[i], words[i + 1]);
            return false;
        }
        }

        // Incrementing index
        i++;
    }

    // Returning true if passed all tests
    return true;
}


/******************** operator_check *******************************/
/* Function takes in a word, returns true if the word is an operator
*  (and/or), false otherwise.
*/
bool operatorCheck(char* word)
{
    if ((strcmp(word, "and")) == 0 || (strcmp(word, "or")) == 0) {
        return true;
    }
    return false;
}


/******************** queryHandler *******************************/
/* Function takes in an index and an array of pointers to words
*  (which has been parsed from the commandline) and handles the
*  conjunction/disjunction of words as we progress through the 
*  query. The function checks for NULL parameters.
*  The function returns the resulting counter (containing the
*  scores from the query).
*/
counters_t* queryHandler(index_t* index, char** words)
{
    // Checking for NULL parameters
    if (index == NULL || words == NULL) {
        return NULL;
    }

    // Initalizing a counter pointer to temp
    counters_t* temp = NULL;

    // Initilializing a result counter
    counters_t* result = counters_new();

    // Iterating through array of words
    int i = 0;
    while (words[i] != NULL) {
        // If the current word is an "and", continue to next word
        if (strcmp(words[i], "and") == 0) {
            i++;
            continue;
        }

        // If the current word is an "or"
        if (strcmp(words[i], "or") == 0) {
            // Calling the disjunction merge function
            disjunctionMerge(result, temp);

            // Deleting temp and setting it to NULL
            counters_delete(temp);
            temp = NULL;
        }

        // If any other word than "and"/"or"
        else {
            // Creating a copy of the counters at current word
            counters_t* copy = countersCopy(index_find(index, words[i]));

            // If temp has not been assigned
            if (temp == NULL) {
                // Setting temp equal to copy
                temp = copy;
            }

            else {
                // Calling the conjunction merge function
                counters_t* tempCopy = conjunctionIntersect(temp, copy);
                
                // Deleting the previous temp
                counters_delete(temp);
                
                // Updating temp to the new copy
                temp = tempCopy;
                
                // Freeing the copied counter
                counters_delete(copy);
            }
        }

        // Advancing through the words array
        i++;
    }

    // Merging contents of temp to result
    disjunctionMerge(result, temp);

    // Deleting the temp
    counters_delete(temp);

    // Returning the resulting counter
    return result;
}


/******************** disjunctionMerge *******************************/
/* Function is responsible for handling cases with 'or' commands
*  (disjunctions). The function takes in two counters as parameters, 
*  calls the iterator for a counters, passing in the helper function. 
*/
void disjunctionMerge(counters_t* result, counters_t* temp)
{
    // Calling the counters iterator, passing in the helper
    counters_iterate(temp, result, disjunctionMergeHelper);
}


/******************** disjunctionMergeHelper *******************************/
/* Function acts as a helper function for the disjunctionMerge function.
*  For each docID in temp, we check if the docID is in result (arg).
*  If it is, we save the sum of the counts in result. If not, we insert
*  the count from temp in result.
*/
void disjunctionMergeHelper(void* arg, const int key, const int count) 
{
    // Saving key in variable docID
    int docID = key;

    // Saving result as the arg
    counters_t* result = arg;

    // Looking for the current docID in temp in result
    int resultCount;

    // If the docID is in result
    if ((resultCount = counters_get(result, docID)) != 0) {
        // Setting the count in result to be the sum of counts
        counters_set(result, docID, count + resultCount);
    }

    // If the docID is in result
    else {
        // Inserting the docID and corresponding count in result
        counters_set(result, docID, count);
    }
}


/******************** conjunctionIntersect *******************************/
/* Function is responsible for handling cases with 'and' commands
*  (conjunctions). The function takes in two counters as parameters, 
*  calls the iterator for a counters, passing in the helper function.
*  It returns the resulting counter from the operation. 
*/
counters_t* conjunctionIntersect(counters_t* temp, counters_t* copy)
{   
    // Creating counter for result
    counters_t* result = counters_new();

    // Creating counterStruct to store result and copy
    counterStruct_t* counterStruct = counterStruct_new(result, copy);

    // Calling the counters iterator, passing in the helper
    counters_iterate(temp, counterStruct, conjunctionIntersectHelper);

    // Cleaning up and returning
    mem_free(counterStruct);

    return result;
}


/******************** conjunctionIntersectHelper *******************************/
/* Function acts as a helper function for the conjunctionIntersect function.
*  For each docID in temp, we check if the docID is in result (arg).
*  If it is, we save find the minimum counts in result and temp and
*  insert it in result. If not, we set the count to 0.
*/
void conjunctionIntersectHelper(void* arg, const int key, const int count) 
{
    // Saving key in variable docID
    int docID = key;

    // Saving counterStruct as the arg
    counterStruct_t* counterStruct = arg;

    // Saving variables from struct
    counters_t* copy = counterStruct->copy;
    counters_t* result = counterStruct->result;
    
    // Looking for the current docID in temp in result
    int copyCount;

    // Finding the minimum count across the counters
    int minCount = 0;

    // If the docID is in copy
    if ((copyCount = counters_get(copy, docID)) != 0) {
        if (count <= copyCount) {
            minCount = count;
        } else {
            minCount = copyCount;
        }
    }

    // Setting the count in result to be the least of the counts (or 0)
    counters_set(result, docID, minCount);
}

/******************** countersCopy *******************************/
/* Function is responsible for copying the contents of the original
*  counter into a copy. This is done to avoid manipulating the 
*  counter in the index when performing merging operations. The
*  function takes in an original counter as parameter, creating
*  a copy of it by calling the iterator with countersCopyHelper.
*/
counters_t* countersCopy(counters_t* counters)
{
    // Creating new counters to copy into
    counters_t* new_counters = counters_new();

    // Calling the iterator on the counters passed to the function
    counters_iterate(counters, new_counters, countersCopyHelper);

    // Returning the copied counters
    return new_counters;
}

/******************** countersCopyHelper *******************************/
/* Function is a helper for the counters_iterate function in the
*  countersCopy function. The function inserts the key and count from the
*  original counters into the copy. 
*/
void countersCopyHelper(void* arg, const int key, const int count)
{
    // Saving the arg as a counter
    counters_t* new_counters = arg;
    
    // Inserting the key and count (from original counter) into the copy
    counters_set(new_counters, key, count);
}

/******************** countersCount *******************************/
/* Funtion takes in a counter object. If not NULL, we loop through
*  the counter, counting the amount of keys in the counter object.
*  The counting itself is done by the helper function. We return
*  the count.
*/
int countersCount(counters_t* counter)
{
    // Initialize a size variable
    int keyCount = 0;
    
    // Call counters_iterate and pass size as the arg
    counters_iterate(counter, (void*)&keyCount, countersCountHelper);

    // Returning count
    return keyCount;
}


/**************** countersCountHelper *******************************/
/* Function acts as a helper function to the countersCount function. 
*  It increments the keyCount variable for every iteration (if 
*  the count is not 0). 
*/
void countersCountHelper(void* arg, const int key, const int count)
{   
    // Incrementing the keyCount if the count is not 0
    if (count != 0) {
        // Saving arg as keyCount, incrementing
        int* keyCount = (int*)arg;
        (*keyCount)++; 
    }
}


/**************** queryOutput *******************************/
/* Function is responsible for the overarching process of
*  sorting the scores in the result counter (by calling other
*  related functions). The function takes in a counter as
*  parameter, having been produced through the conjunction/
*  disjunctions in the commandline. Additionally, it takes in
*  the crawler-produced pageDirectory to access webpage 
*  information The function prints out the ranked results 
*  from performing the query. 
*/
void queryOutput(counters_t* result, char* pageDirectory)
{
    // Checking the size of the results (if there are any)
    int keyCount = countersCount(result);

    // If there are results
    if (keyCount > 0) {
        fprintf(stdout, "Matches %d documents (ranked):\n", keyCount);
    } 
    
    // If there are no results
    else {
        fprintf(stdout, "No documents match.\n");
        // Printing separator for next query
        fprintf(stdout, "-----------------------------------------------\n");
        return;
    }

    // Looping over results
    int i = 0;
    while (i < keyCount) {
        // Retrieving highest ranked element
        rankElement_t* max = resultRanking(result);
        
        // Saving docID variable
        int docID = max->docID;

        // Creating the pathname for the file
        char* pathName = index_pathname(pageDirectory, docID);

        // Opening the file for the corresponding docID
        FILE* file = fopen(pathName, "r");

        // Checking for file NULL pointer
        if (file == NULL) {
            // Printing error message and exiting with non-zero
            fprintf(stderr, "\nError: The memory allocation "
                "was unsuccessful.\n");
            exit(4);
        }

        // Gathering score for current docID
        int score = max->score;

        // Reading URl from file
        char* webpageURL = file_readLine(file);
        
        // Printing out score summary for the docID
        fprintf(stdout, "score %3d doc %3d: %s\n", score, docID, webpageURL);

        // Setting count to 0 for max
        counters_set(result, max->docID, 0);

        // Cleaning up
        mem_free(webpageURL);
        mem_free(pathName);
        fclose(file);
        rankElement_delete(max);

        // Moving on
        i++;
    }

    // Printing separator for next query
    fprintf(stdout, "-----------------------------------------------\n");
}


/**************** resultRanking *******************************/
/* Function takes in the result counter. With the help of the
*  helper method, it iterates through the counter, finding
*  the object with the highest score in the counter, returning
*  it.
*/
rankElement_t* resultRanking(counters_t* result) 
{
    // Initializing a variable for the max value
    rankElement_t* max = rankElement_new(0, 0); 
    
    // Calling the counters_iterate with helper function,
    counters_iterate(result, max, resultRankingHelper);

    // Returning max element
    return max;
}


/**************** resultRankingHelper *******************************/
/* Function is a helper function for the resultRanking function.
*  It checks if the score of the current object is greater than that
*  of the current max. If so, it updates the max. 
*/
void resultRankingHelper(void* arg, const int key, const int count)
{
    // Saving arg as max
    rankElement_t* max = arg;

    // If the count is greater than current max, save current in max
    if (count > max->score) {
        max->docID = key;
        max->score = count;
    }
}
