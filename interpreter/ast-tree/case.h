/* Case.h */

#ifndef _AST_TREE_CASE_H_
#define _AST_TREE_CASE_H_

#include "list.h"
#include "type.h"
#include "statement.h"
#include "scope.h"

typedef enum { RET_INT, RET_STR, RET_OBJ } RetType;

typedef struct Func {
    /* Identifier of the case */
    char *identifier;
    /* Statements within the test case */
    list *statements;
    /* Indicate that what type of function will return */
    RetType type;
    /* Note: Every test case will return true or false */
    Variable (*compute)(struct Func *, Scope *);
} Func;

/* Member function implemented as macros */
#define FUNC_IS_EMPTY_FUNC(F) ((F)->statements == null)
#define FUNC_SET_STATEMENT_LIST(F, L) ((F)->statements = (L))
#define FUNC_COMPUTING(F, S) ((F)->compute(F, S))

/* Prototypes */
Func * funcGenerate();
_Status_t funcAppendStatement(Func *c, Statement *s);

#endif /* _AST_TREE_CASE_H_ */
