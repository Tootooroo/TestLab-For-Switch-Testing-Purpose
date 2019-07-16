#ifndef _AST_TREE_PARAMETER_H_
#define _AST_TREE_PARAMETER_H_

#include "pair.h"
#include "list.h"
#include "scope.h"

typedef struct Expression Expression;

typedef struct Parameter {
    /* pair<ident, type> */
    pair *p;
    Scope *outer;
} Parameter;

typedef struct Argument {
    /* pair<Expression> */
    Expression *expr;
    Scope *outer;
} Argument;

typedef struct Parameters {
    int num;
    /* list<Parameter :: pair<ident, type> */
    list *parameters;
} Parameters;

typedef struct Arguments {
    int num;
    /* list<Argument :: pair<Expression> */
    list *arguments;
} Arguments;

/* Member function implemented as macros */

// Parameter
#define PARAM_TYPE(P) ((char *)PAIR_GET_RIGHT((P)->p))
#define PARAM_SET_TYPE(P, T) (PAIR_SET_RIGHT((P)->p, T))

#define PARAM_IDENT(P) ((char *)PAIR_GET_LEFT((P)->p))
#define PARAM_SET_IDENT(P, I) (PAIR_SET_LEFT((P)->p, I))

#define PARAM_NUM(P) ((P)->num)
#define PARAM_NUM_PLUS(P) ((P)->num++)

// Argument
#define ARGU_EXPR(A) ((A)->expr)
#define ARGU_SET_EXPR(A, E) ((A)->expr = (E))

#define ARGU_NUM(A) ((A)->num)
#define ARGU_NUM_PLUS(A) ((A)->num++)

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

Argument * arguGen(Expression *expr);

/* Push argument into a scope this procedure is for the purpose
 * of easily to store arguments into scope. The reason to pass
 * parameters is give the availibility of type checking. */
_Status_t argusStore(Arguments *, Parameters *, Scope *);

#ifdef _AST_TREE_TESTING_

void paramTest(void **state);

#endif /* _AST_TREE_TESTING_ */



#endif /* _AST_TREE_PARAMETER_H_ */
