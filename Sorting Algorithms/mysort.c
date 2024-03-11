/* CS 107, Assignment 4
 * Louis Weisdorf
 * The program, mysort.c, ontains functionality to sort the inputs of a given
 * file, based on flags passed to the program. As such, it provides a simplied
 * version of the Unix sort command. 
 */
#include <assert.h>
#include <errno.h>
#include <error.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "samples/prototypes.h"

#define MAX_LINE_LEN 4096
#define MIN_NLINES 100

typedef int (*cmp_fn_t)(const void *p, const void *q);

/* The function acts as a comparison function for the sort_lines function, 
 * called in cases without a flag. The function sorts the inputs
 * alphabetically using the strcmp function from the string library. 
 */
int cmp_pstr(const void *p, const void *q) {
    const char *s1 = *(char **)p;
    const char *s2 = *(char **)q;
    return strcmp(s1, s2);
}

/* The function acts as a comparison function for the sort_lines function, 
 * called in cases with an -l flag. The function sorts the inputs based on 
 * their lengths using the strlen function from the string library. 
 */
int cmp_pstr_len(const void *p, const void *q) {
    const char *s1 = *(char **)p;
    const char *s2 = *(char **)q;
    return (strlen(s1) - strlen(s2));
}

/* The function acts as a comparison function for the sort_lines function, 
 * called in cases with an -n flag. The function sorts the inputs based on 
 * their numerical values, using the atoi function. 
 */
int cmp_pstr_numeric(const void *p, const void *q) {
    const char *s1 = *(char **)p;
    const char *s2 = *(char **)q;
    return (atoi(s1) - atoi(s2));
}

/* The function handles the sorting of lines from the input file stream, based
 * on the comparison function, unique, and reverse flags passed to the function.
 * The function reads in lines from the file, heap-allocating them and storing
 * them in a heap-allocated array that resizes when needed. Depending on the
 * reverse flag passed in, the created array of lines is either printed out in
 * non-reverse or reverse order.
 */
void sort_lines(FILE *fp, cmp_fn_t cmp, bool uniq, bool reverse) {
    // Allocating memory for array, initially with MIN_NLINES
    size_t alloc_size = MIN_NLINES;
    char **line_array = malloc(alloc_size * sizeof(char *));
    size_t num_lines_alloc = 0;

    char current_line[MAX_LINE_LEN];

    // Reading lines with fgets into heap array (resizing when necessary)
    while (fgets(current_line, MAX_LINE_LEN, fp)) {
        // If at max capacity, resize array
        if (num_lines_alloc == alloc_size) {
            alloc_size *= 2;
            char **new_line_array = realloc(line_array, alloc_size * sizeof(char *));
            assert(new_line_array);
            line_array = new_line_array;
        }

        // If unique flag applies
        if (uniq) {
            // Calling binsert with pointer to the current line
            char *current_line_ptr = current_line;
            char **retval = binsert(&current_line_ptr, line_array, &num_lines_alloc, sizeof(char *), cmp);

            // If binsert inserted the key, heap allocate current line
            if (*retval == current_line_ptr) {
                char *current_line_cpy = strdup(current_line);
                *retval = current_line_cpy;
            }
        } else {    // If no unique flag is applied
            // Allocating memory for the current line and inserting it into array
            char *current_line_cpy = strdup(current_line);
            line_array[num_lines_alloc] = current_line_cpy;
            num_lines_alloc++;
        }
    }

    // Sorting array according to compare function passed in if no unique flag
    if (!uniq) {
        qsort(line_array, num_lines_alloc, sizeof(char *), cmp);
    }
    
    // If reverse flag applies, print from end to start
    if (reverse) {
        for (int i = num_lines_alloc - 1; i >= 0; i--) {
            char *line = line_array[i];
            // Print line before freeing
            printf("%s", line);
            free(line);
        }
    } else {  // If reverse flag does not apply, printing from start to end
        for (int i = 0; i < num_lines_alloc; i++) {
            char *line = line_array[i];
            printf("%s", line);
            free(line);
        }
    }

    // Freeing line_array
    free(line_array);
}

// ------- DO NOT EDIT ANY CODE BELOW THIS LINE (but do add comments!)  -------

/* The main function handles the executive functioning of the program, 
 * parsing commandline arguments and calling the sort_lines function
 * with the appropriate parameters. 
 */
int main(int argc, char *argv[]) {
    // By default, comparing by alphabet with no u- or r-flags
    cmp_fn_t cmp = cmp_pstr;
    bool uniq = false;
    bool reverse = false;

    // Parsing commandline arguments, setting comparison, u- and r-flags as relevant
    int opt = getopt(argc, argv, "lnru");
    while (opt != -1) {
        if (opt == 'l') {
            cmp = cmp_pstr_len;
        } else if (opt == 'n') {
            cmp = cmp_pstr_numeric;
        } else if (opt == 'r') {
            reverse = true;
        } else if (opt == 'u') {
            uniq = true;
        } else {
            return 1;
        }

        opt = getopt(argc, argv, "lnru");
    }

    // If file passed to the program, opening and checking validity
    FILE *fp = stdin;
    if (optind < argc) {
        fp = fopen(argv[optind], "r");
        if (fp == NULL) {
            error(1, 0, "cannot access %s", argv[optind]);
        }
    }

    // Calling sort_lines and cleaning up
    sort_lines(fp, cmp, uniq, reverse);
    fclose(fp);
    return 0;
}
