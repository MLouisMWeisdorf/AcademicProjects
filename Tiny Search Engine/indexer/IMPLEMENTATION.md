# CS50 TSE Indexer, Louis Weisdorf
## Implementation Spec

In this document, we focus on the core subse of the implementation of the Indexer module of the TSE project:

-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan

## Data structures 

We define the `index` type, which holds a hashtable (initialized to 800 slots). The hashtable, as per default, holds pointers to sets.
Each set represents a word found in the crawler-produced files, which acts as keys in structure. As its item, each set holds a pointer to a counters struct.
The counters structure contains `docID` keys and corresponding counts of the given word in that file.  

## Control flow

The Indexer itself is implemented in one file `indexer.c`, with 3 functions.

### main

The `main` function has several function. Firstly, it parses the arguments passed to the program:

* checks if the number of arguments, `argc`, is 3
* checks if the `pageDirectory` passed to the program is a valid crawler-produced directory, by calling `pagedir_validate`
* checks if the `indexFilename` passed to the program is valid for writing into, by calling `index_filecheck`

If any of these tests fail, the program will exit with a non-zero status specific to the error.

If all tests are successful, the `main` will call `indexBuild`, passing the `pageDirectory` and `indexFilename` to it.

### indexBuild

`indexBuild` builds an in-memory index from webpage files it finds in the pageDirectory.
Pseudocode:

	creates a new 'index' object
	loops over document ID numbers, counting from 1
		loads a webpage from the document file 'pageDirectory/id'
		if successful, 
		passes the webpage and docID to indexPage

### indexPage

`indexPage` scans a webpage document to add its words to the index.
Pseudocode:

	steps through each word of the webpage,
		skips trivial words (less than length 3),
		normalizes the word (converts to lower case),
		looks up the word in the index,
			adding the word to the index if needed
		increments the count of occurrences of this word in this docID

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

The `index.c` module is a re-usable module, stored in the `common` library. The module creates the `index` struct and applies various functions to the struct. Details about each function can be found in the header file for the module, `index.h`.
The functions in the module are described below:

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

We leverage the modules of libcs50, most notably `hashtable`, `counters`, `file`, and `webpage`.
See that directory for module interfaces.
The `webpage` module allows us to represent pages as `webpage_t` objects, which we leverage with the `webpage_GetNextWord` function. This function allows us to loop over each word in the webpage, which we utilize in `indexPage`.
The `file` module allows us to read from files, which we leverage in `pagedir_load`. In this function, we use `file_readLine` and `file_readFile` to extract webpage information from crawler-produced files.

## Function prototypes

### indexer

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `indexer.c` and is not repeated here.

```c
int main(const int argc, char* argv[]);
void indexBuild(char* pageDirectory, char* indexFilename);
void indexPage(index_t* index, webpage_t* webpage, int docID);
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

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

All code uses defensive-programming tactics to catch and exit, e.g., if a function receives bad parameters.

Certain errors are caught and handled internally. For example, the program exits if memory allocation goes wrong at any point in webpage information retrieval, and also if an allocated variable is pointing to NULL.

## Testing plan

Here is an implementation-specific testing plan.

### Unit testing

There are 4 units (`indexer`, `pagedir`, `index`, and `word`).
We perform unit testing on the indexer with the `indextest`, a program which reads an index file into the internal index data structure, then writes the index out to a new file. The `indextest` program relies on functions defined in the `index.c` module, described above.
The other units are small; they could be tested using a small C 'driver' to invoke their functions with various arguments, but it is likely sufficient to observe their behavior during the system test.

### Regression testing

We run the indexer on crawler directories with various size to see if there is a change in performance.

### Integration/system testing

We write a script `testing.sh` that invokes the indexer several times, with a variety of command-line arguments.
First, a sequence of invocations with erroneous arguments, testing each of the possible mistakes that can be made.
Second, a run with indexer on a variety of pageDirectories with indextest as one means of validating the resulting index.
We run that script with `bash -v testing.sh` so the output of indexer is intermixed with the commands used to invoke the indexer.
In addition, we write a `valgrindtest.sh` script, running with valgrind on both indexer and indextest to ensure no memory leaks or errors.
We run that script with `bash -v testing.sh` so the output of valgrind is intermixed with the commands.
We verify correct behavior by studying the output, and by comparing the index output with the files in the respective pageDirectory.
