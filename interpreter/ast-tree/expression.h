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
    Variable (*compute)(struct Expression *, Scope *);
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

// Assignment expression
typedef struct AssignmentExpression {

} AssignmentExpression;

// Constant expression
typedef enum { CONSTANT_INT, CONSTANT_STR } Constant_type;
typedef struct ConstantExpression {
    Expression base;
    Constant_type type;
    union {
        int number;
        char *string;
    };
} ConstantExpression;

#define CONSTANT_GET_INT(C) ((C)->number)
#define CONSTANT_GET_STR(C) ((C)->string)
#define CONSTANT_SET_INT(C, I) ((C)->number = (I))
#define CONSTANT_SET_STR(C, S) ((C)->string = (S))

#define IS_INT_CONSTANT_EXPR(C) ((C)->type == CONSTANT_INT)
#define IS_STR_CONSTANT_EXPR(C) ((C)->type == CONSTANT_STR)

ConstantExpression * constExprDefault();

/* Member functions implement as macros */

/* Prototypes */


#endif /* _AST_TREE_EXPRESSION_H_ */
