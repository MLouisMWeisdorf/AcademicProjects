# CS50 TSE (Indexer Directory)
## Louis Weisdorf (MLouisMWeisdorf)

### Exit codes
1: Exit code for when an invalid number of arguments is given to the commandline. 

2: Exit code for when the `pageDirectory` passed to the commandline is not produced by the crawler.

3: Exit code for when the `indexFilename` passed to the commandline is invalid.

4: Exit code when a malloced variable is pointing to `NULL`.

5: Exit code for when the URL or depth retrieval are unsuccessful in the `pathdir_load`.

6: Exit code for when unable to open oldIndexFilename in indextest.c.

### Notes
The hashtable belonging to the index is created with 800 slots, based on recommendations given in the requirements spec.