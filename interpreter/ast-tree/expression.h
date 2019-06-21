/* expression.h */

#ifndef _AST_TREE_EXPRESSION_H_
#define _AST_TREE_EXPRESSION_H_

#include "variable.h"
#include "scope.h"

typedef enum {
    EXPR_TYPE_ARITHEMETIC,
    EXPR_TYPE_ORDER,
    EXPR_TYPE_MEMBER_REF,
    EXPR_TYPE_FUNC_CALL,
    EXPR_TYPE_CONSTANT,
    EXPR_TYPE_IDENTIFIER
} ExprType;

typedef struct Expression {
    ExprType type;
    Variable * (*compute)(struct Expression *, Scope *);
} Expression;

/* Member function implement as macros */
#define exprType(E) ((E)->type)
#defien exprSetType(E, T) ((E)->type = (T))

typedef struct PlusExpression {} PlusExpression;
typedef struct MinusExpression {} MinusExpression;
typedef struct MulExpression {} MulExpression;
typedef struct DivExpression {} DivExpression;
typedef struct LessThanExpression {} LessThanExpression;
typedef struct GreaterThanExpression {} GreaterThanExpression;
typedef struct EqualExpression {} EqualExpression;
typedef struct LessOrEqualExpression {} LessOrEqualExpression;
typedef struct GreaterOrEqualExpression {} GreaterOrEqualExpression;
typedef struct NotEqualExpression {} NotEqualExpression;
typedef struct MemberSelectExpression {} MemberSelectExpression;
typedef struct FuncCallExpression {} FuncCallExpression;
typedef struct AssignmentExpression {} AssignmentExpression;

#endif /* _AST_TREE_EXPRESSION_H_ */
