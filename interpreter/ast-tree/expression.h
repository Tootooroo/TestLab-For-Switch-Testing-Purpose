/* expression.h */

#ifndef _AST_TREE_EXPRESSION_H_
#define _AST_TREE_EXPRESSION_H_

typedef struct Expression {
    Variable * (*compute)(Expression *);
} Expression;

#endif /* _AST_TREE_EXPRESSION_H_ */
