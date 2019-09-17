/* wrapper.h */

#ifndef _WRAPPER_H_
#define _WRAPPER_H_

#include "type.h"
#include "list.h"

/* Prototypes */
void * zMalloc(size_t size);
int str2Num(char *);
_Bool strCompare(char *, char *);
char * charReplace(char *src, char *c, list *pair_string);
list * strSplit(char *src, char spliter, char end);
char * num2Str(int num);
int numlen(int num);

#ifdef _TEST_LAB_UNIT_TESTING_
void wrapper_test(void **state);
#endif /* _TEST_LAB_UNIT_TESTING_ */

#endif /* _WRAPPER_H_ */

