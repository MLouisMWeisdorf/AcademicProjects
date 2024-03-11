/* CS 107, Assignment 4
 * Louis Weisdorf
 * The file, util.c, contains functionality to perform search on a sorted
 * list, with the added functionality to add the search key if not found.
 */
#include "samples/prototypes.h"
#include <string.h>

/* The generic function contains functionality to perform search on a sorted
 * array, with the added functionality to add the search key if not found.
 * Beyond the array (base) and key, the function takes in a pointer to the
 * number of elements in the array, the width of the type stored, as well as
 * a comparison function. If the key is found in the array, the pointer to
 * the location of the key in the array is returned. If not, the key is inserted
 * into the array, the number of elements is incremented, and a pointer to the
 * location of the key is returned. 
 */
void *binsert(const void *key, void *base, size_t *p_nelem, size_t width, int (*compar)(const void *, const void *)) {
    // Saving intial value of base
    void *start = base;
    size_t nelem = *p_nelem;

    // Looping through array, halving number of remaining elements to search for every iteration
    for (size_t nremain = nelem; nremain != 0; nremain >>= 1) {
        // Locating middle of array
        void *mid_elem = (char *)base + (nremain >> 1) * width;

        // Checking if key is a middle of array
        int sign = compar(key, mid_elem);
        
        // If found key, return key
        if (sign == 0) {
            return mid_elem;
        } 
            
        // If the key is bigger than the middle element, move search to right half of array
        if (sign > 0) {
            base = (char *)mid_elem + width;
            nremain--;
        }

        // If key smaller than middle element, continue searching in left half  
    }

    // Calculating amount of shifts needed for insertion of key
    size_t end = nelem * width;
    size_t base_shifted = (char *)base - (char *)start;
    size_t shift_by = end - base_shifted;

    // Shifting over to make space for insertion and inserting key
    memmove((char *)base + width, base, shift_by);
    memcpy(base, key, width);

    // Updating count after insertion
    *p_nelem = nelem + 1;

    return base;
}
