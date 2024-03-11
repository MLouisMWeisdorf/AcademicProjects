/** 
 * word.c
 * Author: Louis Weisdorf
 * CS50 Spring 2023, Lab 5
 * Date: 04/29/2023
 * Description: This program handles the words on webpages,
 *      preparing them for processing in other functions. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/**************** functions ****************/

/******************** word_check *******************************/
/* See detailed description in word.h. */
bool word_check(char* word)
{
    // If the word is NULL or shorter than 3 characters, returning false
    if (word == NULL || (strlen(word) < 3)) {
        return false;
    }

    // If the word length is 3 characters or more, returning true
    return true;
}

/******************** normalize_word *******************************/
/* See detailed description in word.h. */
char* word_normalize(char* word)
{
    // If the word is NULL, returning NULL
    if (word == NULL) {
        return NULL;
    }

    // Looping over the characters in the word
    for (int i = 0; word[i] != '\0'; i++) {
        // Converting each character to lowercase
        word[i] = tolower(word[i]);
    }

    // Returning the lowercase string
    return word;
}