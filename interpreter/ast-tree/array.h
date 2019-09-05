/* array.h */

#ifndef _AST_TREE_ARRAY_H_
#define _AST_TREE_ARRAY_H_

#include "type.h"
#include "list.h"

typedef struct Variable Variable;

typedef struct Array {
    char *type;

    int numOfElems;
    /* list<Variable> */
    list *elems;
} Array;

/* Member functions implemented as macros */
#define ARRAY_TYPE(A) ((A)->type)
#define ARRAY_SET_TYPE(A, T) ((A)->type = (T))

#define ARRAY_NUM_OF_ELEMS(A) ((A)->numOfElems)
#define ARRAY_SET_NUM_OF_ELEMS(A, N) ((A)->numOfElems = (N))
#define ARRAY_NUM_OF_ELEMS_PLUS(A) ARRAY_SET_NUM_OF_ELEMS(A, ARRAY_NUM_OF_ELEMS(A) + 1)
#define ARRAY_NUM_OF_ELEMS_DESC(A) ARRAY_SET_NUM_OF_ELEMS(A, ARRAY_NUM_OF_ELEMS(A) - 1)

/* Prototypes */
Array * createArray(char *type);
void    arrayRelease(Array *);
Variable  * arraySelectViaIdx(Array *, int index);
void      * arraySetViaIdx(Array *, Variable *, int index);

void       arrayPush(Array *, Variable *);
Variable * arrayPop(Array *);
void       arrayAppend(Array *, Variable *);
Variable * arrayRetrive(Array *);
Array * arrayDup(Array *);

#ifdef _AST_TREE_TESTING_

void arrayTesting(void **state);

#endif /* _AST_TREE_TESTING_ */

#endif /* _AST_TREE_ARRAY_H_ */
