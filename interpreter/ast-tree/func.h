#ifndef _AST_TREE_CASE_H_
#define _AST_TREE_CASE_H_

#include "list.h"
#include "type.h"
#include "scope.h"
#include "variable.h"
#include "pair.h"
#include "parameter.h"

typedef struct Statement Statement;

typedef enum { FUNC_INTERNAL, FUNC_NON_INTERNAL } FuncType;
typedef enum { RET_INT, RET_STR, RET_OBJ } RetType;

typedef Variable * (*internalProc)(void *, Scope *);
typedef struct Func {
    /* Identifier of the case */
    char *identifier;
    /* Statements within the test case */
    union {
        list *statements;
        internalProc interRtn;
    };
    /* Indicate that what type of function will return */
    char *type;
    Parameters *params;
    /* Note: Every test case will return true or false */
    Variable * (*compute)(struct Func *, Scope *);
    /* Scope of function, this scope is determine while
     * functions define. */
    Scope *outer;
    /* Scope where the function to be called */
    Scope *current;
    FuncType fType;
} Func;

/* Member function implemented as macros */
#define FUNC_IS_EMPTY_FUNC(F) ((F)->statements == null)

#define FUNC_SET_IDENT(F, I) ((F)->identifier = (I))
#define FUNC_IDENT(F) ((F)->identifier)

#define FUNC_STATEMENT_LIST(F) ((F)->statements)
#define FUNC_SET_STATEMENT_LIST(F, L) ((F)->statements = (L))

#define FUNC_RETURN_TYPE(F) ((F)->type)
#define FUNC_SET_RETURN_TYPE(F, R) ((F)->type = R)

#define FUNC_COMPUTING(F, S) ((F)->compute(F, S))
#define FUNC_SET_STATEMENTS(F, S) ((F)->statements = (S))

#define FUNC_SET_ENV(F, E) ((F)->outer = (E))
#define FUNC_ENV(F) ((F)->outer)

#define FUNC_PARAMETERS(F) ((F)->params)
#define FUNC_IS_INTERNAL(F) ((F)->fType == FUNC_INTERNAL)

#define FUNC_SET_TYPE(F, TYPE) ((F)->fType = (TYPE))

/* Prototypes */
Func * funcGenerate();
Func * funcGen(char *ident, char *type, Parameters *, Scope *s);
void funcRelease(Func *);

/* Procedure to add parameter call with the first
 * parameter before the second */
_Status_t funcAddParam(Func *, Parameter *);
Parameter * funcGetParamByName(Func *, char *);
/* If you want to get first parameter give i = 1, second
 * parameter give i = 2 and so on. */
Parameter * funcGetParamByPos(Func *, int i);


/* Procedure to append statement to function, the statement
 * append earlier will be executed earlier. */
_Status_t funcAppendStatements(Func *c, Statement *s);

#ifdef _AST_TREE_TESTING_

void funcTest(void **state);

#endif /* _AST_TREE_TESTING_ */

#endif /* _AST_TREE_CASE_H_ */
