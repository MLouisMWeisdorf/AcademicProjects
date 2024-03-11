/* CS 107, Assignment 4
 * Louis Weisdorf
 * The program, myls.c, is a simplified version of the Unix ls command.
 * The program takes in 0 or more arguments, as well as two potential flags,
 * and prints out the directory entries from each path.
 */
#include <dirent.h>
#include <error.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* A typedef is a way to give a nickname to a type that is easier to use.  For example,
 * this typedef means you can now use filter_fn_t as the type for a variable, parameter,
 * or return type of this type, in place of the longhand form.  This can come in
 * particularly handy when storing functions in variables.  E.g. instead of
 *
 * int (*myfunc)(const struct_dirent *) = my_fn_name;
 *
 * you can do:
 *
 * filter_fn_t myfunc = my_fn_name;
 *
 * See your C reference or web search for more on typedef.  Consider making other
 * typedefs where appropriate to make your code cleaner!
 */
typedef int (*filter_fn_t)(const struct dirent *);
typedef int (*cmp_fn_t)(const struct dirent **, const struct dirent **);

/* Enums are used here as a way to declare 4 integer constants we can use
 * in our program.  This gives us similar benefits to #defining each of these
 * 4 integer constants, but enums allow us to group the constants
 * together and auto-assign them values, since we don't really care what value
 * they are, we just care that they are assigned distinct values and that
 * we can use them in our code.  See your C reference or web search for more on enums!
 */
enum { SORT_BY_NAME, SORT_BY_TYPE };
enum { EXCLUDE_DOT, INCLUDE_DOT };

/* This fully implemented function returns whether the dirent pointed to by
 * the given pointer represents a directory.  (Note: on the myth filesystem,
 * the only file type information that is accurate is directory/not-directory
 * used here. Other type info in struct dirent is not reliable).  Note that
 * this implementation categorizes symbolic links, such as the samples/ directory
 * in the assignment folder, as not a directory, which is fine, and this matches
 * the behavior of the sample solution.
 */
bool is_dir(const struct dirent *dirptr) {
    return dirptr->d_type == DT_DIR;
}

/* The function acts as the comparison function for the scandir function called
 * in ls in cases without the -z flag (order input is 0). The function
 * sorts the input dirent struct double pointers alphabetically using the
 * strcmp function from the string library.  
 */
int compare_alpha(const struct dirent **f1, const struct dirent **f2) {
    // Extracting file name from input structs and returning strcmp result of the two
    const char *first_file_name = (*f1)->d_name;
    const char *second_file_name = (*f2)->d_name;
    return strcmp(first_file_name, second_file_name);
}

/* The function acts as the comparison function for the scandir function called
 * in ls in cases when the -z flag is applied (order input is 1). The function
 * takes in double pointers to dirent structs, sorting directories ahead of 
 * files or calling compare_alpha function if both inputs are either 
 * directories or files.  
 */
int compare_type(const struct dirent **f1, const struct dirent **f2) {
    // If f1 is a directory and f2 is a file
    if (is_dir(*f1) && !is_dir(*f2)) {
        return -1;
    } else if (!is_dir(*f1) && is_dir(*f2)) {   // Opposite above
        return 1;
    }

    // If both files are either directories or files, compare alphabetically
    return compare_alpha(f1, f2);
}

/* The function acts as the filtering function for the scandir function called
 * in ls. The function checks if the first character of the pathname is a dot,
 * returning 0 if so (to exclude) or 1 if not (to include).
 */
int filter_dot(const struct dirent *path) {
    // If first character is a '.', do not include
    if (path->d_name[0] == '.') {
        return 0;
    } else {    // Otherwise, include
        return 1;
    }
}

/* The function takes in a directory path, as well as parameters specifying the
 * filter and order to apply to the output. Using the scandir function, the 
 * function extracts the files in the given directory path. If the filter input 
 * is 0, the function excludes files with a leading '.'. If the filter input is
 * 1, these files are included. If the order input is 0, the order of the print
 * output is strictly alphabetical. If the order input is 1, directories are
 * printed before files (and alphabetically sorted within the groups). 
 */
void ls(const char *dirpath, int filter, int order) {
    struct dirent **file_list = NULL;
    int num_files = 0;

    // Setting filter based on input passed to function
    filter_fn_t filter_input = (filter == 0) ? filter_dot : NULL;

    // Setting comparison based on input passed to function 
    cmp_fn_t comparison_input = (order == 0) ? compare_alpha : compare_type;

    // Calling scandir with filter and comparison input functions
    num_files = scandir(dirpath, &file_list, filter_input, comparison_input);

    // If error occurred, printing error and returning
    if (num_files == -1) {
        error(0, 0, "cannot access %s", dirpath);
        return;
    }

    // Iterating through and printing file and directory names
    for (int i = 0; i < num_files; i++) {
        struct dirent *current_file = file_list[i];

        // Checking if file is directory, adding trailing slash
        if (is_dir(current_file)) {
            printf("%s/\n", current_file->d_name);
        } else {
            printf("%s\n", current_file->d_name);
        }
        free(current_file);
    }
    free(file_list);
}

// ------- DO NOT EDIT ANY CODE BELOW THIS LINE (but do add comments!)  -------

/* The main function handles the executive functioning of the program, 
 * parsing commandline arguments and calling the ls function
 * with the appropriate parameters. 
 */
int main(int argc, char *argv[]) {
    // The order and filter default to sorting by name and excluding the leading dot
    int order = SORT_BY_NAME;
    int filter = EXCLUDE_DOT;

    // Parsing commandline arguments, setting filter and order variables if relevant
    int opt = getopt(argc, argv, "az");
    while (opt != -1) {
        if (opt == 'a') {
            filter = INCLUDE_DOT;
        } else if (opt == 'z') {
            order = SORT_BY_TYPE;
        } else {
            return 1;
        }

        opt = getopt(argc, argv, "az");
    }

    // If several directories are passed in, printing results for each separately (calling ls function)
    if (optind < argc - 1) {
        for (int i = optind; i < argc; i++) {
            printf("%s:\n", argv[i]);
            ls(argv[i], filter, order);
            printf("\n");
        }
    } else {
        ls(optind == argc - 1 ? argv[optind] : ".", filter, order);
    }

    return 0;
}
