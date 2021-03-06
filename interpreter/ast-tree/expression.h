/* expression.h */

#ifndef _AST_TREE_EXPRESSION_H_
#define _AST_TREE_EXPRESSION_H_

#include "variable.h"
#include "scope.h"
#include "parameter.h"

typedef enum {
    EXPR_TYPE_ARITHEMETIC,
    EXPR_TYPE_ORDER,
    EXPR_TYPE_MEMBER_REF,
    EXPR_TYPE_FUNC_CALL,
    EXPR_TYPE_CONSTANT,
    EXPR_TYPE_IDENTIFIER,
    EXPR_TYPE_ASSIGN
} ExprType;

typedef struct Expression {
    ExprType type;
    Variable * (*compute)(struct Expression *, Scope *);
    void (*release)(struct Expression *, Scope *);
} Expression;

/* Member function implement as macros */
#define EXPR_2_BASE(E) ((Expression *)(E))
#define exprType(E) (EXPR_2_BASE(E)->type)
#define exprSetType(E, T) (EXPR_2_BASE(E)->type = (T))
#define exprCompute(E, S) (EXPR_2_BASE(E)->compute(EXPR_2_BASE(E), S))
#define exprRelease(E, S) (EXPR_2_BASE(E)->release(EXPR_2_BASE(E), S))

// Plus Expression
typedef struct PlusExpression {
    Expression base;
    Expression *left;
    Expression *right;
} PlusExpression;

#define PLUS_EXPR_SET_LEFT(E, L) ((E)->left = (L))
#define PLUS_EXPR_SET_RIGHT(E, R) ((E)->right = (R))

PlusExpression * plusExprDefault();
PlusExpression * plusExprGen(Expression *left, Expression *right);
void plusExprRelease(Expression *, Scope *);

// Minus Expression
typedef struct MinusExpression {
    Expression base;
    Expression *left;
    Expression *right;
} MinusExpression;

#define MINUS_EXPR_SET_LEFT(E, L) ((E)->left = (L))
#define MINUS_EXPR_SET_RIGHT(E, R) ((E)->right= (R))

MinusExpression * minusExprDefault();
MinusExpression * minusExprGen(Expression *left, Expression *right);
void minusRelease(Expression *expr, Scope *s);


// Multiplication Expression
typedef struct MulExpression {
    Expression base;
    Expression *left;
    Expression *right;
} MulExpression;

#define MUL_EXPR_SET_LEFT(E, L) ((E)->left = (L))
#define MUL_EXPR_SET_RIGHT(E, R) ((E)->right = (R))

MulExpression * mulExprDefault();
MulExpression * mulExprGen(Expression *left, Expression *right);
void mulExprRelease(Expression *expr, Scope *s);

// Division expression
typedef struct DivExpression {
    Expression base;
    Expression *left;
    Expression *right;
} DivExpression;

#define MUL_EXPR_SET_LEFT(E, L) ((E)->left = (L))
#define MUL_EPXR_SET_RIGHT(E, R) ((E)->right = (R))

DivExpression * divExprDefault();
DivExpression * divExprGen(Expression *left, Expression *right);
void divExprRelease(Expression *expr, Scope *scope);

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
void lessThanExprRelease(Expression *left, Scope *s);

// Greater than expression
typedef struct GreaterThanExpression {
    Expression base;
    Expression *left;
    Expression *right;
} GreaterThanExpression;

#define GREATER_THAN_SET_LEFT(E, L) ((E)->left = (L))
#define GREATER_THAN_SET_RIGHT(E, R) ((E)->right = (R))

GreaterThanExpression * greaterThanExprDefault();
GreaterThanExpression * greaterThanExprGen(Expression *left, Expression *right);
void greaterThanExprRelease(Expression *, Scope *);

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
void equalExprRelease(Expression *, Scope *);

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
void lessOrEqualExprRelease(Expression *expr, Scope *s);

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
void greaterOrEqualRelease(Expression *expr, Scope *s);

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
void notEqualExprRelease(Expression *expr, Scope *s);

// Member Select expression
typedef struct MemberSelectExpression {
    Expression base;
    Expression *head;
    char *member;
} MemberSelectExpression;

#define MEMBER_SELECT_HEAD(M) ((M)->head)
#define MEMBER_SELECT_SET_HEAD(M, H) ((M)->head = (H))

#define MEMBER_SELECT_SET_SUBS(M, MEM) ((M)->member = (MEM))
#define MEMBER_SELECT_SUBS(M) ((M)->member)

MemberSelectExpression * memberSelectDefault();
MemberSelectExpression * memberSelectGen(Expression *objName, char *member);
void memberSelectRelease(Expression *, Scope *);

// Function call expression
typedef struct FuncCallExpression {
    Expression base;
    char *funcIdent;
    /* The first argument is environment argument, a reference
     * refer to current scope. */
    Arguments *args;
} FuncCallExpression;

#define FUNC_CALL_IDENT(F) ((F)->funcIdent)
#define FUNC_CALL_SET_IDENT(F, I) ((F)->funcIdent = (I))

#define FUNC_CALL_ARGUMENT(F) ((F)->args)
#define FUNC_CALL_SET_ARGUL(F, L) ((F)->arguments = (L))

FuncCallExpression * funcCallExprDefault();
FuncCallExpression * funcCallExprGen(char *ident, Arguments *arguments);
void funcCallRelease(Expression *, Scope *);
_Status_t funcCallAddArg(Expression *, Argument *);

// Assignment expression
typedef struct AssignmentExpression {
    Expression base;

    /* Left value */
    Expression *l;
    Expression *r;
} AssignmentExpression;

#define ASSIGN_GET_LEFT(A) ((A)->l)
#define ASSIGN_SET_LEFT(A, L) ((A)->l = (L))

#define ASSIGN_GET_RIGHT(A) ((A)->r)
#define ASSIGN_SET_RIGHT(A, R) ((A)->r = (R))

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
ConstantExpression * constExprGen(void *value, primitiveType type);
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
void identExprRelease(Expression *expr, Scope *s);

/* Member functions implement as macros */

/* Prototypes */

// Index expression
typedef struct IndexExpression {
    Expression base;
    Expression *ident;
    Expression *index;
} IndexExpression;

#define IDX_EXPR_IDENT(IDX_E) ((IDX_E)->ident)
#define IDX_EXPR_SET_IDENT(IDX_E, IDENT) ((IDX_E)->ident = IDENT)

#define IDX_EXPR_IDX(IDX_E) ((IDX_E)->index)
#define IDX_EXPR_SET_IDX(IDX_E, IDX) ((IDX_E)->index = IDX)

IndexExpression * indexExprGen(Expression *ident, Expression *expr);

// Array expression
typedef struct ArrayExpression {
    Expression base;
    list *elements_expr;
} ArrayExpression;

/* Member functions implement as macros */
#define ARRAY_EXPR_ELEMENTS(AE) ((AE)->elements_expr)
#define ARRAY_EXPR_SET_ELEMENTS(AE, E) ((AE)->elements_expr = (E))

ArrayExpression * arrayExprGen(list *exprs);

// Percent expression
typedef struct PercentExpression {
    Expression base;
    Expression *formatString;
    list *valExprs;
} PercentExpression;

/* Member functions implement as macros */
#define PERCENT_EXPR_FORMAT_STR(PE) ((PE)->formatString)
#define PERCENT_EXPR_SET_FORMAT_STR(PE, STR_EXPR) ((PE)->formatString = (STR_EXPR))

#define PERCENT_EXPR_V_EXPRS(PE) ((PE)->valExprs);
#define PERCENT_EXPR_SET_V_EXPRS(PE, V) ((PE)->valExprs = (V))

PercentExpression * percentExprGen(Expression *formatStr, list *valExprs);

#ifdef _AST_TREE_TESTING_

void exprTest(void **state);

#endif /* _AST_TREE_TESTING_ */

#endif /* _AST_TREE_EXPRESSION_H_ */
