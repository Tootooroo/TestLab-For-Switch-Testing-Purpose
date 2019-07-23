/* wrapper.h */

#ifndef _WRAPPER_H_
#define _WRAPPER_H_

#include "type.h"
#include "list.h"

/* Prototypes */
void * zMalloc(size_t size);
int str2Num(char *);
_Bool strCompare(char *, char *);
list * strSplit(char *src, char spliter, char end);

#endif /* _WRAPPER_H_ */

