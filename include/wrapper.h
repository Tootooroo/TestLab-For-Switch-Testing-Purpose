/* wrapper.h */

#ifndef _WRAPPER_H_
#define _WRAPPER_H_

#include "type.h"
#include "list.h"

/* Prototypes */
void * zMalloc(size_t size);
int str2Num(char *);
_Bool strCompare(char *, char *);
char * strReplace(char *src, list *pair_list);
list * strSplit(char *src, char spliter, char end);
char * num2Str(int num);
int numlen(int num);

#endif /* _WRAPPER_H_ */

