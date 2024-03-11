# CS50 TSE (Querier Directory)
## Louis Weisdorf (MLouisMWeisdorf)

### Functionality implemented
I have implemented the full functionality, meaning that my querier prints the document set in decreasing order by score, thus meeting the full specs.

### Exit codes
1: Exit code for when an invalid number of arguments is given to the commandline. 

2: Exit code for when the `pageDirectory` passed to the commandline is not produced by the crawler.

3: Exit code for when the `indexFile` passed to the commandline is not readable. 

4: Exit code when a malloced variable is pointing to `NULL`.

### Assumptions and notes
Since the files we are working with in this project are produced by the crawler and indexer programs, respectively, 
there is not additional extensive error checking on these file when processed in the querier program. This is done based on the
assumption that any errors would have been caught in the two preceding programs. 
Other than this, the program utilizes defensive programming principles to avoid unexpected outcomes.
