/* statement.h */

#ifndef _AST_TREE_STATEMENT_H_
#define _AST_TREE_STATEMENT_H_

#include "type.h"
#include "expression.h"
#include "list.h"

typedef enum {
    IF_STATEMENT_ID,
    RETURN_STATEMENT_ID,
    VAR_DECL_STATEMENT_ID,
    FUNC_DECL_STATEMENT_ID,
    EXPR_STATEMENT_ID,
    ASSIGN_STATEMENT_ID
} StatementID;

typedef struct StatementTrack {
    /* pointer reference to the statement which
     * return this track */
    void *s;
    StatementID id;
    /* field v is used to hold value
     * return by a return statement */
    void *v;
} StatementTrack;

/* Base statement structure */
typedef struct Statement {
    StatementTrack (*compute)(Statement *);
} Statement;

typedef struct IfStatement {
    Statement base;
    Expression *conditionExpr;
    Statement *trueStatement;
    Statement *falseStatement;
} IfStatement;

typedef struct ReturnStatement {
    Statement base;
    Expression *expr;
} ReturnStatement;

typedef struct VarDeclStatement {
    Statement base;
    /* list of pair(identifier, constantExpre) */
    list *varDeclExprs;
} VarDeclStatement;



#endif /* _AST_TREE_STATEMENT_H_ */
