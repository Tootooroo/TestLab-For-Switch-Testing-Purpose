/* epression.c */

#include "expression.h"

/* Private prototypes */
Variable constExprCompute(Expression *, Scope *);
Variable assignExprCompute(Expression *, Scope *);
Variable memberSelectExprCompute(Expression *, Scope *);
Variable funcCallExprCompute(Expression *, Scope *);

/* Public procedures */

// Constant expression
ConstantExpression * constExprDefault() {
    ConstantExpression *expr = (ConstantExpression *)zMalloc(sizeof(ConstantExpression));
    expr->base.compute = constExprCompute;

    return expr;
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

/* Private procedures */
Variable constExprCompute(Expression *expr, Scope *scope) {
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

Variable assignExprCompute(Expression *expr, Scope *scope) {

}

Variable memberSelectExprCompute(Expression *expr, Scope *scope) {}

Variable funcCallExprCompute(Expression *expr, Scope *scope) {}
