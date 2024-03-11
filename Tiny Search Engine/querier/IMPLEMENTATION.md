# CS50 TSE Querier, Louis Weisdorf
## Implementation Spec

In this document, we focus on the core subse of the implementation of the Indexer module of the TSE project:

-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan

## Data structures 

We define the `index` type, which holds a hashtable (initialized to the amount of lines (words) in the index file). The hashtable, as per default, holds pointers to sets.
Each set represents a word found in the crawler-produced files, which acts as keys in structure. As its item, each set holds a pointer to a counters struct.
The counters structure contains `docID` keys and corresponding counts of the given word in that file. 
In addition, we create a new type `rankElement`, which stores a docID and score within the struct. This type is used for ranking the search results.  
We also create a new type `counterStruct`, which stores two counters. We use this type in the queryHandler, more specifically to intersect counters. 

## Control flow

The Querier itself is implemented in one file `querier.c`, with 3 functions.

### main

The `main` function has several function. Firstly, it parses the arguments passed to the program:

* checks if the number of arguments, `argc`, is 3
* checks if the `pageDirectory` passed to the program is a valid crawler-produced directory, by calling `pagedir_validate`
* checks if the `indexFilename` passed to the program is valid for reading from

If any of these tests fail, the program will exit with a non-zero status specific to the error.

If all tests are successful, the `main` will initialize an new index to be worked with from there on out, call `inputHandler`, passing the `index` and `indexFilename` to it.

### inputHandler

`inputHandler` builds an in-memory index from webpage files it finds in the pageDirectory.
Pseudocode:

	until the stdin reached EOF
        prompt a query
        read input
        parse input into tokens
        validate tokens
        call query_handler with index and tokens
        call sorter_output_function with resulting counter
        clean up

### parseLine

`parseLine` parses a line read from the stdin into single words with a null terminator.
Pseudocode:

	initialize word count variable
	set variables to track position in line
	set variables to track position in word array
	set pointer to start of word
	set pointer to rest of word
	set boolean to determine if working on new word, true first
	step through each character in the line
		if the current character is not an alphabet or space (invalid character), return 0 with error message.
		make character lowercase
		if we are working on a new word
			if the current character is not a space 
				set start and rest pointer to character
		else
			if current character is a space and current word is not empty
				add null terminator and add to array of words
				increment word count
	if current word is not empty at the end of the line (no concluding space)
		add to array of words
		increment word count
	return word count

### inputValidate

`inputValidate` iterates over the words tokenized from stdin, checking for illegal structures or words.
Pseudocode:

	if the first word is an operator
		print error message, return false.
	if the last word is an operator
			print error message, return false.
	looping over words
		if the current word is shorter than 3 characters and not an operator
			print error message, return false.
		if there are two consecutive operators
			print error message, return false.
	return true if passed all tests.

### operatorCheck

`operatorCheck` checks if a word is an operator.
Pseudocode:

	if the word is an operator, return true.
	else, return false.

### queryHandler

`queryHandler` checks if a word is an operator.
Pseudocode:

	initializing temp counter
	initializing result counter
	looping over words
		if word is "and"
			continue to next word
		if word is "or"
			call function to merge result and temp counters
			deleting and setting temp to NULL
		if any other word
			make copy of the counters found at the current word in index
			if temp is NULL
				set temp to copy
			else
				call function to intersect temp and copy counters, save result in tempCopy
				delete temp
				set temp to tempCopy
				delete copy counter
	merge contents of result and temp
	delete temp counter
	return result counter.

### disjunctionMerge

`disjunctionMerge` calls iterator to merge counters.
Pseudocode:

	call counters iterator with helper function, passing in two counters

### disjunctionMergeHelper

`disjunctionMergeHelper` performs actions of merging counters (as helper function to `disjunctionMerge`).
Pseudocode:

	save result as the arg
	if the current docID in temp is in result
		set the count in result to be the sum of the counts across counters
	else 
		insert the docID and corresponding count from temp in result
	
### conjunctionIntersect

`conjunctionIntersect` calls iterator to intersect counters.
Pseudocode:

	create a result counter
	create a counterStruct object to store result and copy
	call counters iterator with helper function, passing in temp and counterStruct
	clean up and return.

### conjunctionIntersectHelper

`conjunctionIntersectHelper` performs actions of intersecting counters (as helper function to `conjunctionIntersect`).
Pseudocode:

	save counterStruct as the arg
	saving counters from counterStruct in variables copy and result
	initializing minCount to 0
	if the current docID in temp is in copy
		find the smallest count across temp and copy corresponding to the docID
	set the smallest count as the count for the docID in result

### countersCopy

`countersCopy` copies the contents of a counter into another.
Pseudocode:

	create new counter
	call counter iterator with helper function
	return new counter

### countersCopyHelper

`countersCopyHelper` does the actual work of copying a counter into another (as helper function for `countersCopy`).
Pseudocode:

	save the new counter as arg
	set the key and count of the original counter into the new

### countersCount

`countersCount` calls iterator to count number of docID keys in a counter.
Pseudocode:

	initialize variable to count keys
	call counters iterator with helper function
	return key count

### countersCountHelper

`countersCountHelper` does the actual work of counting the number of keys in a counter (as helper function for `countersCount`).
Pseudocode:

	if the count is not 0
		save key count as arg
		increment key count

### queryOutput

`queryOutput` prints query results to the stdout, having called other functions to receive ranked results.
Pseudocode:

	save keycount from counting function
	if there are results
		print information about number of results
	if there are no results
		print appropriate message and return.
	looping over results
		retrieving result with highest score
		retrieve information from crawler file
		print information
		set score of max result to 0 (for it to not be in consideration in next iteration)

### resultRanking

`resultRanking` finds the result with the highest score.
Pseudocode:

	create new rankElement object max with docID,score of 0,0 to hold best result
	call counter iterator, passing in rankElement
	returning max rankElement

### resultRankingHelper

`resultRankingHelper` does the work of comparing scores and updating rankElement information (as a helper function for `resultRanking`).
Pseudocode:

	save rankElement max as arg
	if score of current docID is greater than the score of the current max
		update information for max (docID and score)

### rankElement_new

`rankElement_new` initializes a rankElement struct, setting its properties according to the parameters passed in.
Pseudocode:

	allocate memory for element
	if memory allocation failed
		return NULL.
	else
		set properties according to parameters
		return element.

### rankElement_delete

`rankElement_delete` deletes a rankElement by freeing the memory allocated for it.
Pseudocode:

	if element is not NULL
		free the element.

### counterStruct_new

`counterStruct_new` initializes a rankElement struct, setting its properties according to the parameters passed in.
Pseudocode:

	allocate memory for struct
	if memory allocation failed
		return NULL.
	else
		set properties according to parameters
		return struct.

### prompt

`prompt` prints the query prompt if stdin is a keyboard (this function was provided in the assignment description).
Pseudocode:

	if the stdin is a keyboard
		print query prompt

## Other modules

### pagedir

The `pagedir.c` module is a re-usable module, stored in the `common` library, originally written for the Crawler program. In the Indexer program, however, we make additions to this module. Details about each function can be found in the header file for the module, `pagedir.h`.
The pre-existing functions are described below:

Pseudocode for `pagedir_init`:

	construct the pathname for the .crawler file in that directory
	open the file for writing; on error, return false.
	close the file and return true.


Pseudocode for `pagedir_save`:

	construct the pathname for the page file in pageDirectory
	open that file for writing
	print the URL
	print the depth
	print the contents of the webpage
	close the file


For the Indexer program, we make use of the following added functions in the `pagedir` module.

Pseudocode for `pagedir_validate`:

	construct the pathname for a .crawler file in the given pageDirectory
	open the file for reading; on error, return false.
	close the file and return true.


Pseudocode for `pagedir_load`:

	read the webpage URL from the first line of the file
	read depth from the second line of the file
	read HTML from thir line on
	if above retrieval was unsuccessful, exit with non-zero.
	convert depth string to int
	create new webpage struct from retrived information
	if above creation was unsuccessful, exit with non-zero.
	return the webpage.


### index

The `index.c` module is a re-usable module, stored in the `common` library. The module creates the `index` struct and applies various functions to the struct. Details about each function can be found in the header file for the module, `index.h`. The module was originally written for the indexer program, but functionalities have been added for the querier program.
The pre-existing functions in the module are described below:

Pseudocode for `index_new`:

	allocate memory for the index struct
	if allocation is unsuccessful, return NULL.
	create a new hashtable to be part of the index struct
	return the index.


Pseudocode for `index_insert`:

	if index passed to the function is NULL, return false.
	extract the counters object from hashtable the with the given word
	if there is no key in the counter for the given word
		create a new counters object
		insert it into the hashtable
	add or increment count of counter belonging to docID
	return true.


Pseudocode for `index_filecheck`:

	open the file for writing
	if the file is NULL, return NULL.
	close the file and return true.


Pseudocode for `index_pathname`:

	create string to store the docID, exit with non-zero if NULL.
	create string to store the pathName, exit with non-zero if NULL.
	copy pathDirectory contents into pathName
	concatenate a slash and the ID onto the pathName, return it.


Pseudocode for `index_file_read`:

	open the index file passed to the function
	if file is NULL, exit with non-zero.
	loop over the lines in the file
		save the first word in the line as the word (key in index hashtable)
		initilialize counter
		initialize variables to progress along line
		loop over remaining elements in the line
			if at a docID
				save as docID
			if at count
				save as count
				call counters_set with docID and corresponding count
		insert into hashtable in index


Pseudocode for `index_write`:

	check if indexFile is valid for writing
	open the indexFile for writing
	call the iterator for the hashtable in the index, pass in an itemfunc for the set
	close the file
	delete the index


Pseudocode for `set_iterate_helper`:

	open the file, print the word int it
	call the iterator for the counters, pass in itemfunc for the counter
	print a newline


Pseudocode for `counters_iterate_helper`:

	print the docID and corresponding count into the file


Pseudocode for `index_delete`:

	if the index passed in is not NULL
		call delete function on the hashtable in the index, pass in delete function for counters as itemdelete


Pseudocode for `counters_delete_helper`:

	delete counters passed in

The added functionality:

Pseudocode for `index_find`:

	in a counter object, save the result of the hashtable find function on the word passed in
	return the counter object

### word

The `word.c` module is a re-usable module, stored in the `common` library. The module checks the length of words and normalizes them (converts them to lowercase). Details about each function can be found in the header file for the module, `word.h`.
The functions in the module are described below:

Pseudocode for `word_check`:

	if the word is NULL or shorter than 3 characters, return false.
	else, return true.


Pseudocode for `word_normalize`:

	if the word is NULL, return NULL.
	loop over each character in the word
		convert the character to lowercase
	return the word.


### libcs50

We leverage the modules of libcs50, most notably `hashtable`, `counters`, `file`.
See that directory for module interfaces.
The `file` module allows us to read from files, which we leverage in throughout the querier program (to read lines, count number of lines, etc.)
The `word` module is used in the parsing and validating of strings to normalize them and check their lenght.
The `counters` module is used extensively in the program, namely to keep track of query results. 

## Function prototypes

### querier

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `querier.c` and is not repeated here.

```c
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
```

### pagedir

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `pagedir.h` and is not repeated here.

```c
bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
bool pagedir_validate(char* pageDirectory);
webpage_t* pagedir_load(FILE* file); 
```

### index

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `index.h` and is not repeated here.

```c
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
```

### word

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `word.h` and is not repeated here.

```c
bool word_check(char* word);
char* word_normalize(char* word);
```

## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; non-recoverable problems result in a message printed to stderr and a non-zero exit status.

All code uses defensive-programming tactics to catch and exit, e.g., if a function receives bad parameters.

Certain errors are caught and handled internally. For example, the program exits if memory allocation goes wrong at any point in webpage information retrieval, and also if an allocated variable is pointing to NULL.

In many cases, errors with the current query do not result in exiting the program. Instead, they are handled with error statements, whereafter a new query is prompted.

## Testing plan

Here is an implementation-specific testing plan.

### Unit testing

There are no single modules in the `querier` program that are specifically tested. Rather, the testing is done as integration testing for the entire program. 
The modules used in the program, e.g. `index`, `pagedir`, and `word` are smaller programs that are not tested. 

### Regression testing

We run the querier on indixes with various size, as well as with different number of queries, to see if there is a change in performance.

### Integration/system testing

We write a script `testing.sh` that invokes the querier several times, with a variety of command-line arguments.
First, a sequence of invocations with erroneous arguments, testing each of the possible mistakes that can be made.
Second, a run with querier on a variety of crawler pageDirectory/indexer indexFile pairs, with search queries produced by the `fuzzquery.c` program.
We run that script with `bash -v testing.sh` so the output of querier is intermixed with the commands used to invoke the querier.
In addition, we write a `valgrind_test.sh` script, running with valgrind on the cases from the testing script to ensure no memory leaks or errors.
We run that script with `bash -v valgrind_test.sh` so the output of valgrind is intermixed with the commands.
We verify correct behavior by studying the output, and by comparing the query output with the corresponding indexFile. 
