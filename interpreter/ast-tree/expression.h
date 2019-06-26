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
#define exprSetType(E, T) ((E)->type = (T))
#define exprCompute(E, S) ((E)->compute(E, S))

typedef struct PlusExpression {

} PlusExpression;
typedef struct MinusExpression {} MinusExpression;
typedef struct MulExpression {} MulExpression;
typedef struct DivExpression {} DivExpression;
typedef struct LessThanExpression {} LessThanExpression;
typedef struct GreaterThanExpression {} GreaterThanExpression;
typedef struct EqualExpression {} EqualExpression;
typedef struct LessOrEqualExpression {} LessOrEqualExpression;
typedef struct GreaterOrEqualExpression {} GreaterOrEqualExpression;
typedef struct NotEqualExpression {} NotEqualExpression;

// Member Select expression
typedef struct MemberSelectExpression {
    Expression base;
    Expression *headExpr;
    list *subs;
} MemberSelectExpression;

#define MEMBER_SELECT_HEAD(M) ((M)->head)
#define MEMBER_SELECT_SET_HEAD(M, H) ((M)->head = (H))
#define MEMBER_SELECT_SUBS(M) ((M)->subs)

MemberSelectExpression * memberSelectDefault();
void memberSelectAppendSub(MemberSelectExpression *, char *);

// Function call expression
typedef struct FuncCallExpression {
    Expression base;
    char *funcIdent;
    /* list of argument's names
     * compute procedure will
     * search these arguments in
     * current scope. */
    list *arguments;
} FuncCallExpression;

#define FUNC_CALL_SET_IDENT(F, I) ((F)->funcIdent = (I))
#define FUNC_CALL_SET_ARGUL(F, L) ((F)->arguments = (L))

FuncCallExpression * funcCallExprDefault();
FuncCallExpression * funcCallExprGen(char *ident, list *arguments);

// Assignment expression
typedef enum { ASSIGN_L_IDENT, ASSIGN_L_MEMBER } Assign_l_type;
typedef struct AssignmentExpression {
    Expression base;

    /* Left value */
    Expression *l;
    Expression *r;
} AssignmentExpression;

#define ASSIGN_IS_L_IDENT(A) ((A)->type == ASSIGN_L_IDENT)
#define ASSIGN_IS_L_MEMBER(A) ((A)->type == ASSIGN_L_MEMBER)
#define ASSIGN_GET_IDENT(A) ((A)->ident)
#define ASSIGN_GET_MEMBER(A, S) ((A)->expr)

/* Prototypes */
AssignmentExpression * assignExprDefault();
AssignmentExpression * assignExprGen(void *left, Expression *right);
void assignExprRelease(Expression *);

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

// Identifier expression
typedef struct IdentifierExpression {

} IdentifierExpression;

/* Member functions implement as macros */

/* Prototypes */


#endif /* _AST_TREE_EXPRESSION_H_ */
