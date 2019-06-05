/* statement.h */

#ifndef _AST_TREE_STATEMENT_H_
#define _AST_TREE_STATEMENT_H_

#include "expression.h"

/* Base statement structure */
typedef struct Statement {
    void (*compute)(Statement *);
} Statement;

typedef struct IfStatement {
    Statement base;
    Expression *conditionExpr;
    Statement *trueStatement;
    Statement *falseStatement;
} IfStatement;

#endif /* _AST_TREE_STATEMENT_H_ */
