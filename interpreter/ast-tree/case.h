/* Case.h */

#ifndef _AST_TREE_CASE_H_
#define _AST_TREE_CASE_H_

#include "list.h"
#include "type.h"
#include "scope.h"
#include "variable.h"
#include "pair.h"

typedef struct Statement Statement;

typedef enum { RET_INT, RET_STR, RET_OBJ } RetType;

typedef pair Parameter;

typedef struct Parameters {
    int num;
    /* list<ident, type> */
    list *parameters;
} Parameters;

typedef struct Func {
    /* Identifier of the case */
    char *identifier;
    /* Statements within the test case */
    list *statements;
    /* Indicate that what type of function will return */
    char *type;
    Parameters *params;
    /* Note: Every test case will return true or false */
    Variable * (*compute)(struct Func *, Scope *);
    /* Scope of function, this scope is determine while
     * functions define. */
    Scope *outer;
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

#define PARAM_TYPE(P) ((char *)PAIR_GET_RIGHT(P))
#define PARAM_SET_TYPE(P, T) (PAIR_SET_RIGHT(P, T))

#define PARAM_IDENT(P) ((char *)PAIR_GET_LEFT(P))
#define PARAM_SET_IDENT(P, I) (PAIR_SET_LEFT(P, I))

/* Prototypes */
Func * funcGenerate();
Func * funcGen(Parameters *);

/* Procedure to add parameter call with the first
 * parameter before the second */
_Status_t funcAddParam(Func *, Parameter *);

/* Procedure to append statement to function, the statement
 * append earlier will be executed earlier. */
_Status_t funcAppendStatements(Func *c, Statement *s);

/* Generate an empty parameters */
Parameters * paramsGen();

/* Procedure to add parameter call with the first
 * parameter before the second */
_Status_t paramsAdd(Parameters *, Parameter *);

Parameter * paramsGetByName(Parameters *, char *ident);

/* If you want to get first parameter give i = 1, second
 * parameter give i = 2 and so on. */
Parameter * paramsGetByPos(Parameters *p, int i);

Parameter * paramGen(char *ident, char *type);

#endif /* _AST_TREE_CASE_H_ */
