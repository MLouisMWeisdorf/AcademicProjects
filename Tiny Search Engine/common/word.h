/** 
 * word.h
 * Author: Louis Weisdorf
 * CS50 Spring 2023, Lab 5
 * Date: 04/29/2023
 * Description: This is a header file for the index.c program.
 */
#include <stdio.h>

/**************** functions ****************/

/******************** word_check *******************************/
/* Function takes in a word.
*  If the word is NULL or shorter than 3 characters, returns
*  false. Else, it returns true. 
*/
bool word_check(char* word);

/******************** normalize_word *******************************/
/* Function takes in a word.
*  If the word is NULL, function returns NULL.
*  Otherwise, the function loops over the characters in the word,
*  making them lowercase, before returning the lowercase word.
*/
char* word_normalize(char* word);