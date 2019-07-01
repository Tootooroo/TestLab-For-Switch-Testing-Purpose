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
    void (*release)(struct Expression *, Scope *);
} Expression;

/* Member function implement as macros */
#define exprType(E) ((E)->type)
#define exprSetType(E, T) ((E)->type = (T))
#define exprCompute(E, S) ((E)->compute(E, S))
#define exprRelease(E, S) ((E)->release(E, S))

// Plus Expression
typedef struct PlusExpression {
    Expression base;
    Expression *left;
    Expression *right;
} PlusExpression;

#define PLUS_EXPR_SET_LEFT(E, L) ((E)->left = (L))
#define PLUS_EXPR_SET_RIGHT(E, R) ((E)->right = (R))

PlusExpression * plusExprDefault();
PlusExpression * plusStmtGen(Expression *left, Expression *right);

// Minus Expression
typedef struct MinusExpression {
    Expression base;
    Expression *left;
    Expression *right;
} MinusExpression;

#define MINUS_EXPR_SET_LEFT(E, L) ((E)->left = (L))
#define MINUS_EXPR_SET_RIGHT(E, R) ((E)->right= (R))

MinusExpression * minusStmtDefault();
MinusExpression * minusStmtGen(Expression *left, Expression *right);

// Multiplication Expression
typedef struct MulExpression {
    Expression base;
    Expression *left;
    Expression *right;
} MulExpression;

#define MUL_EXPR_SET_LEFT(E, L) ((E)->left = (L))
#define MUL_EXPR_SET_RIGHT(E, R) ((E)->right = (R))

MulExpression * mulStmtDefault();
MulExpression * mulStmtGen(Expression *left, Expression *right);

// Division expression
typedef struct DivExpression {
    Expression base;
    Expression *left;
    Expression *right;
} DivExpression;

#define MUL_EXPR_SET_LEFT(E, L) ((E)->left = (L))
#define MUL_EPXR_SET_RIGHT(E, R) ((E)->right = (R))

DivExpression * divStmtDefault();
DivExpression * divStmtGen(Expression *left, Expression *right);

// Less than expression
typedef struct LessThanExpression {
    Expression base;
    Expression *left;
    Expression *right;
} LessThanExpression;

#define LESS_THAN_SET_LEFT(E, L) ((E)->left = (L))
#define LESS_THAN_SET_RIGHT(E, R) ((E)->right = (R))

LessThanExpression * lessThanExprDefault();
LessThanExpression * lessThanExprGen(Expression *left, Expression *right);

typedef struct GreaterThanExpression {
    Expression base;
    Expression *left;
    Expression *right;
} GreaterThanExpression;

#define GREATER_THAN_SET_LEFT(E, L) ((E)->left = (L))
#define GREATER_THAN_SET_RIGHT(E, R) ((E)->right = (R))

GreaterThanExpression * greaterThanExprDefault();
GreaterThanExpression * greaterhanExprGen(Expression *left, Expression *right);

// Equal expression
typedef struct EqualExpression {
    Expression base;
    Expression *left;
    Expression *right;
} EqualExpression;

#define EQUAL_EXPR_SET_LEFT(E, L) ((E)->left = (L))
#define EQUAL_EXPR_SET_RIGHT(E, R) ((E)->right = (R))

EqualExpression * equalExprDefault();
EqualExpression * equalExprGen(Expression *left, Expression *right);

// Less or equal expression
typedef struct LessOrEqualExpression {
    Expression base;
    Expression *left;
    Expression *right;
} LessOrEqualExpression;

#define LESS_OR_EQUAL_EXPR_SET_LEFT(E, L) ((E)->left = (L))
#define LESS_OR_EQUAL_EXPR_SET_RIGHT(E, R) ((E)->right = (R))

LessOrEqualExpression * lessOrEqualExprDefault();
LessOrEqualExpression * lessOrEqualExprGen(Expression *left, Expression *right);

// Greater or equal expression
typedef struct GreaterOrEqualExpression {
    Expression base;
    Expression *left;
    Expression *right;
} GreaterOrEqualExpression;

#define GREATER_OR_EQUAL_EXPR_SET_LEFT(E, L) ((E)->left = (L))
#define GREATER_OR_EQUAL_EXPR_SET_RIGHT(E, R) ((E)->right = (R))

GreaterOrEqualExpression * greaterOrEqualExprDefault();
GreaterOrEqualExpression * greaterOrEqualGen(Expression *left, Expression *right);

// Not Equal expression
typedef struct NotEqualExpression {
    Expression base;
    Expression *left;
    Expression *right;
} NotEqualExpression;

#define NOT_EQUAL_EXPR_SET_LEFT(E, L) ((E)->left = (L))
#define NOT_EQUAL_EXPR_SET_RIGHT(E, R) ((E)->right = (R))

NotEqualExpression * notEqualExprDefault();
NotEqualExpression * notEqualExprGen(Expression *left, Expression *right);

// Member Select expression
typedef struct MemberSelectExpression {
    Expression base;
    Expression *head;
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
typedef struct AssignmentExpression {
    Expression base;

    /* Left value */
    Expression *l;
    Expression *r;
} AssignmentExpression;

#define ASSIGN_IS_L_IDENT(A) ()
#define ASSIGN_IS_L_MEMBER(A) ()
#define ASSIGN_GET_IDENT(A) ()
#define ASSIGN_GET_MEMBER(A, S) ()

/* Prototypes */
AssignmentExpression * assignExprDefault();
AssignmentExpression * assignExprGen(Expression *left, Expression *right);
void assignExprRelease(Expression *, Scope *scope);

// Constant expression
typedef enum { CONSTANT_INT, CONSTANT_STR } Constant_type;
typedef struct ConstantExpression {
    Expression base;
    Constant_type type;
    Variable *constant_var;
} ConstantExpression;

#define IS_INT_CONSTANT_EXPR(C) ((C)->type == CONSTANT_INT)
#define IS_STR_CONSTANT_EXPR(C) ((C)->type == CONSTANT_STR)
#define CONSTANT_GET_VAR(C) ((C)->constant_var)

ConstantExpression * constExprDefault();
_Status_t constExprSetInt(ConstantExpression *expr, int num);
_Status_t constExprSetStr(ConstantExpression *expr, char *str);
void constExprRelease(ConstantExpression *expr, Scope *scope);

// Identifier expression
typedef struct IdentExpression {
    Expression base;
    char *ident;
} IdentExpression;

#define IDENT_EXPR_SET(E, I) ((E)->ident = (I))
#define IDENT_EXPR_GET(E) ((E)->ident)

IdentExpression * identExprDefault();
IdentExpression * identExprGen(char *);

/* Member functions implement as macros */

/* Prototypes */


#endif /* _AST_TREE_EXPRESSION_H_ */
