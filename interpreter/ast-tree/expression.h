/* expression.h */

#ifndef _AST_TREE_EXPRESSION_H_
#define _AST_TREE_EXPRESSION_H_

#include "variable.h"
#include "scope.h"

typedef struct Expression {
    Variable * (*compute)(struct Expression *, Scope *);
} Expression;

#endif /* _AST_TREE_EXPRESSION_H_ */
