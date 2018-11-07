/**
 *Custom ADT for both the tree and htable programs.
 *consists of methods that deal with both programs such as
 *memory allocation
 */

#ifndef MYLIB_H_
#define MYLIB_H_

#include <stddef.h>

extern void *emalloc(size_t);
extern void *erealloc(void *, size_t);
extern int getword(char *s, int limit, FILE *stream);

#endif
