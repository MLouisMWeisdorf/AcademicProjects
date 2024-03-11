# CS50 TSE (Crawler Directory)
## Louis Weisdorf (MLouisMWeisdorf)

### Exit codes
1: Exit code for when an invalid number of arguments is given to the command line. 

2: Exit code when the `seedURL` provided is not an internal URL.

3: Exit code for when the `maxDepth` provided is not valid (not an integer or not in range).

4: Exit code when a malloced variable is pointing to `NULL`.

### Notes
In the `pageScan` method, the structure of the while loop is inspired by the usage notes in the
header file for the `webpage.c` program.

Although the primary argument checking is done in `parseArgs`, I am doing some initial checking in `main`.
I am checking if the argument passed as `maxDepth` is an integer. I found this necessary since I did not wish to
pass along a non-integer to the `parseArgs` and eventually the `crawl` functions. 

No assumptions were made beyond those in the Requirements Spec.

The implementation does not differ from the provided Specs.

### Testing
Testing is performed with the shell script `testing.sh`, which can be called with `make test`.
The testing is comprised of four tests with erroneous arguments (testing the argument-checking),
followed by two test-runs of each file in the TSE playground index 
(`letters`, `toscrape`, and `wikipedia`). Testing was done at further depths previously, but the depths
have been reduced to avoid submission of large files.

Valgrind testing is performed with `valgrind.sh`, which can be called with `make valgrind`.
The trial program run in `valgrind.sh` displays a short run (`wikipedia` with `maxDepth` of 0) 
to avoid excessive run time.
