/* epression.c */

#include "expression.h"

/* Private prototypes */
Private Variable constExprCompute(Expression *, Scope *);
Private Variable assignExprCompute(Expression *, Scope *);
Private Variable memberSelectExprCompute(Expression *, Scope *);
Private Variable funcCallExprCompute(Expression *, Scope *);
Private Variable identExprCompute(Expression *expr, Scope *scope);

// Operators
Private Variable plusStmtCompute(Expression *expr, Scope *scope);
Private Variable minusStmtCompute(Expression *expr, Scope *scope);

/* Public procedures */

// Constant expression
ConstantExpression * constExprDefault() {
    ConstantExpression *expr = (ConstantExpression *)zMalloc(sizeof(ConstantExpression));
    expr->base.compute = constExprCompute;

    return expr;
}

_Status_t constExprSetInt(ConstantExpression *expr, int num) {
    Primitive *int_pri = primitiveGenerate_i(num);
    Variable *var = varGen(NULL, VAR_PRIMITIVE, int_pri);

    expr->constant_var = var;

    return OK;
}

_Status_t constExprSetStr(ConstantExpression *expr, char *str) {
    Primitive *str_pri = primitiveGenerate_s(str);
    Variable *var = varGen(NULL, VAR_PRIMITIVE, str_pri);

    expr->constant_var = var;

    return OK;
}

// Asignment expression
AssignmentExpression * assignExprDefault() {
    AssignmentExpression *aExpr = (AssignmentExpression *)zMalloc(sizeof(AssignmentExpression));
    aExpr->base.compute = assignExprCompute;

    return aExpr;
}

AssignmentExpression * assignExprGen(void *left, Expression *right, Assign_l_type type) {
    AssignmentExpression *aExpr = assignExprDefault();
    aExpr->ident = (char *)left;
    aExpr->rVal = right;
    aExpr->type = type;

    return aExpr;
}

void assignExprRelease(Expression *expr) {}

// Member select expression
MemberSelectExpression * memberSelectDefault() {
    MemberSelectExpression *mExpr = (MemberSelectExpression *)zMalloc(sizeof(MemberSelectExpression));
    mExpr->base.compute = memberSelectCompute;

    return mExpr;
}

void memberSelectAppendSub(MemberSelectExpression *expr, char *sub) {
    if (expr->subs == null)
        expr->subs = listCreate();

    listAppend(expr->subs, sub);
}

// Function call expression
FuncCallExpression * funcCallExprDefault() {
    FuncCallExpression *f = (FuncCallExpression *)zMalloc(sizeof(FuncCallExpression));
    f->base.compute = funcCallExprCompute;
    return f;
}

FuncCallExprEssion * funcCallExprGen(char *ident, list *arguments) {
    FuncCallExpression *f = funcCallExprDefault();
    f->funcIdent = ident;
    f->arguments = arguments;

    return f;
}

// Identifier expression
IdentExpression * identExprDefault() {
    IdentExpression *iExpr = (IdentExpression *)zMalloc(sizeof(IdentExpression));
    iExpr->base.compute = identExprCompute;

    return iExpr;
}

IdentExpression * identExprGen(char *ident) {
    IdentExpression *iExpr = identExprDefault();
    iExpr->ident = ident;

    return iExpr;
}

// Plus expression
PlusExpression * plusStmtDefault() {
    PlusExpression *expr = (PlusExpression *)zMalloc(sizeof(PlusExpression));
    expr->base.compute = plusStmtComput;
    return expr;
}

PlusExpression * plusStmtGen(Expression *left, Expression *right) {
    PlusExpression *expr = plusStmtDefault();
    PLUS_EXPR_SET_LEFT(expr, left);
    PLUS_EXPR_SET_RIGHT(expr, right);

    return expr;
}

// Minus expression
MinusExpression * minusStmtDefault() {
    MinusExpression *expr = (PlusExpression *)zMalloc(sizeof(MinusExpression));
    expr->base.compute = minusStmtCompute;
    return expr;
}

MinusExpression * minusStmtGen(Expression *left, Expression *right) {
    MinusExpression *expr = minusStmtDefault();
    MINUS_EXPR_SET_LEFT(expr, left);
    MINUS_EXPR_SET_RIGHT(expr, right);
    return expr;
}

/* Private procedures */
Private Variable constExprCompute(Expression *expr, Scope *scope) {
    ConstantExpression *cExpr = (ConstantExpression *)expr;
    Variable var = VarDefault_Empty();
    Primitive *p;

    if (IS_INT_CONSTANT_EXPR(cExpr)) {
        p = primitiveGenerate_i(CONSTANT_GET_INT(cExpr));
    } else if (IS_STR_CONSTANT_EXPR(cExpr)) {
        p = primitiveGenerate_s(CONSTANT_GET_STR(cExpr));
    }
    VAR_SET_RPIMITIVE(&var, p);

    return var;
}

Private Variable assignExprCompute(Expression *expr, Scope *scope) {}

Private Variable memberSelectExprCompute(Expression *expr, Scope *scope) {}

Private Variable funcCallExprCompute(Expression *expr, Scope *scope) {}

// Operators
Private Variable identExprCompute(Expression *expr, Scope *scope) {
    IdentExpression *iExpr = expr;

    char *ident = iExpr->ident;

    Primitive *p = scopeGetPrimitive(scope, ident);
    if (p) return varGen(strdup(ident), VAR_PRIMITIVE, p);

    Object *o = scopeGetObject(scope, ident);
    if (o) return varGen(strdup(ident), VAR_OBJECT, o);

    // Can not find the identifer in the current scope.
    return varDefault_Empty();
}

Private Variable plusStmtCompute(Expression *expr, Scope *scope) {
    Expression *left = expr->left, *right = expr->right;

    return varPlusOp(left.compute(left, scope), right.compute(right, scope));
}

Private Variable minusStmtCompute(Expression *expr, Scope *scope) {
    Expression *left = expr->left, *right = expr->right;

    return varMinusOp(left.compute(left, scope), right.compute(right, scope));
}
