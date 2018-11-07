/**
 *This program implements the custom ADT for the methods
 *that are used in both the tree and htable programs
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "mylib.h"

/**
 *This function gets the word from the file stream.
 *@param s is the pointer to the char array
 *@param limit is the maximum length of the word
 *@param stream is the stream we are getting the word from
*/
int getword(char *s, int limit, FILE *stream) {
    int c;
    char *w = s;
    assert(limit > 0 && s != NULL && stream != NULL);
    /* skip to the start of the word */
    while (!isalnum(c = getc(stream)) && EOF != c)
        ;
    if (EOF == c) {
        return EOF;
    } else if (--limit > 0) { /* reduce limit by 1 to allow for the \0 */
        *w++ = tolower(c);
    }
    while (--limit > 0) {
        if (isalnum(c = getc(stream))) {
            *w++ = tolower(c);
        } else if ('\'' == c) {
            limit++;
        } else {
            break;
        }
    }
    *w = '\0';
    return w - s;
}

/**
 *This function allocates requested memory and returns a pointer to it.
 *@param s is the size of memory to allocate.
 *@return pointer to the new memory found by the system.
*/
void *emalloc(size_t s) {
    void *p = malloc(s);
    if(NULL==p){
        fprintf(stderr, "Memory Allocation Failed!\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

/**
 *Reallocates memory pointed to by p and returns a pointer to the new memory.
 *@param p This is the pointer to a memory block previously allocated with emalloc
 *or realloc to be reallocated. If this is NULL, a new block is allocated and a pointer to it
 *is returned by the function.
 *@param s this is the new size for the memory block.
 *@return pointer to the memory.
*/
void *erealloc(void *p, size_t s){
    void *tmp = realloc(p, s);
    if(NULL==tmp){
        fprintf(stderr, "Memory Allocation Failed!\n");
        exit(EXIT_FAILURE);
    }
    return tmp;
}


