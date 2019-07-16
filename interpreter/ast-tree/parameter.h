#ifndef _AST_TREE_PARAMETER_H_
#define _AST_TREE_PARAMETER_H_

#include "pair.h"
#include "list.h"

typedef struct Expression Expression;

typedef pair Parameter;
typedef pair Argument;

typedef struct Parameters {
    int num;
    /* list<ident, type> */
    list *parameters;
} Parameters;

typedef struct Arguments {
    int num;
    /* list<Expression, type> */
    list *arguments;
} Arguments;

/* Member function implemented as macros */

// Parameter
#define PARAM_TYPE(P) ((char *)PAIR_GET_RIGHT(P))
#define PARAM_SET_TYPE(P, T) (PAIR_SET_RIGHT(P, T))

#define PARAM_IDENT(P) ((char *)PAIR_GET_LEFT(P))
#define PARAM_SET_IDENT(P, I) (PAIR_SET_LEFT(P, I))

// Argument
#define ARGU_TYPE(A) ((char *)PAIR_GET_RIGHT(A))
#define ARGU_SET_TYPE(A, T) (PAIR_SET_RIGHT(A, T))

#define ARGU_EXPR(A) ((char I)PAIR_GET_LEFT(A))
#define ARGU_SET_EXPR(A, E) (PAIR_GET_LEFT(A, E))

/* Prototypes */

// Parameter
Parameters * paramsGen();

/* Procedure to add parameter call with the first
 * parameter before the second */
_Status_t paramsAdd(Parameters *, Parameter *);
void paramsRelease(Parameters *);

Parameter * paramsGetByName(Parameters *, char *ident);

/* If you want to get first parameter give i = 1, second
 * parameter give i = 2 and so on. */
Parameter * paramsGetByPos(Parameters *p, int i);

Parameter * paramGen(char *ident, char *type);
void paramRelease(Parameter *);

// Argument
Arguments * argusGen();
void argusRelease(Arguments *);

_Status_t argusAdd(Arguments *, Argument *);
/* If you want to get first parameter give i = 1, second
 * parameter give i = 2 and so on. */
Argument * argusGetByPos(Arguments *, int i);

Argument * arguGen(Expression *expr, char *type);

#ifdef _AST_TREE_TESTING_

void paramTest(void **state);

#endif /* _AST_TREE_TESTING_ */



#endif /* _AST_TREE_PARAMETER_H_ */
