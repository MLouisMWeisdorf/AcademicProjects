# CS50 TSE (Common Directory)
## Louis Weisdorf (MLouisMWeisdorf)

### Notes
The implementation does not differ from the provided Specs.

In `index.c`, we check on two occasions (prior to starting the indexing process in `main` and in the beginning of `index_write`) that the indexFile is valid for writing into. We do not perform this check at every stage of the process of writing into the indexFile (through `set_iterate_helper` and `counter_iterate_helper`), making the assumption that the file does not somehow become invalid during this process.
