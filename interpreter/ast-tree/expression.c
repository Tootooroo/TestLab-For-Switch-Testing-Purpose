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
Private Variable mulStmtCompute(Expression *expr, Scope *scope);
Private Variable divStmtCompute(EXpression *expr, Scope *scope);
Private Variable lessThanExprCompute(Expression *expr, Scope *scope);
Private Variable greaterThanExprCompute(Expression *expr, Scope *scope);
Private Variable equalExprCOmpute(Expression *expr, Scope *scope);
Private Variable lessOrEqualExprCompute(Expression *expr, Scope *scope);
Private Variable greaterOrEqualCompute(Expression *expr, Scope *scope);
Private Variable notEqualCompute(Expression *expr, Scope *scope);

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

// Mul expression
MulExpression * mulStmtDefault() {
    MulExpression *mExpr = (MulExpression *)zMalloc(sizeof(MulExpression));
    mExpr->base.compute = mulStmtCompute;
    return mExpr;
}

MulExpression mulStmtGen(Expression *left, Expression *right) {
    MulExpression *mExpr = mulStmtDefault();
    MUL_EXPR_SET_LEFT(mExpr, left);
    MUL_EXPR_SET_RIGHT(mExpr, right);

    return mExpr;
}

// Div expression
DivExpression * divStmtDefault() {
    DivExpression *dExpr = (DivExpression *)zMalloc(sizeof(DivExpression));
    dExpr->base.compute = divStmtCompute;
    return dExpr;
}

DivExpression * divStmtDefault(Expression *l, Expression *r) {
    DivExpression *dExpr = divStmtDefault();
    MUL_EXPR_SET_LEFT(dExpr, l);
    MUL_EXPR_SET_RIGHT(dExpr, r);
    return dExpr;
}

// Less thant expression
LessThanExpression * lessThanExprDefault() {
    LessThanExpression *lExpr = (LessThanExpression *)zMalloc(sizeof(LessThanExpression));
    lExpr->base.compute = lessThanStmtcompute;
    return lExpr;
}

LessThanExpression * lessThanExprGen(Expression *left, Expression *right) {
    LessThanExpression *lExpr = lessThanExprDefault();
    LESS_THAN_SET_LEFT(lExpr, left);
    LESS_THAN_SET_RIGHT(lExpr, right);

    return lExpr;
}

// Greater than expression
GreaterThanExpression * greaterThanExprDefault() {
    GreaterThanExpression *gExpr = (GreaterThanExpression *)zMalloc(sizeof(GreaterThanExpression));
    gExpr->base.compute = greaterThanExprCompute;
    return gExpr;
}

GreaterThanEexpression * greaterThanExprGen(Expression *left, Expression *right) {
    GreaterThanExpression *gExpr = greaterThanExprDefault();
    GREATER_THAN_SET_LEFT(gExpr, left);
    GREATER_THAN_SET_RIGHT(gExpr, right);
    return gExpr;
}

// Equal expression
EqualExpression * equalExprDefault() {
    EqualExpression *expr = (EqualExpression *)zMalloc(sizeof(EqualExpression));
    expr->base.compute = equalExprCompute;

    return expr;
}

EqualExpression * equalExprGen(Expression *left, Expression *right) {
    EqualExpression *expr = equalExprDefault();
    EQUAL_EXPR_SET_LEFT(expr, left);
    EQUAL_EXPR_SET_RIGHT(expr, right);

    return expr;
}

// Less or equal expression
LessOrEqualExpression * lessOrEqualExprDefault() {
    LessOrEqualExpression *expr = (LessOrEqualExpression *)zMalloc(sizeof(LessOrEqualExpression));
    expr->base.compute = lessOrEqualExprCompute;
    return expr;
}

LessOrEqualExpression * lessOrEqualExprGen(Expression *left, Expression *right) {
    LessOrEqualExpression *expr = lessOrEqualExprDefualt();
    LESS_OR_EQUAL_EXPR_SET_LEFT(expr, left);
    LESS_OR_EQUAL_EXPR_SET_RIGHT(expr, right);

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

Private Variable mulStmtCompute(Expression *expr, Scope *scope) {
    Expression *left = expr->left, *right = expr->right;

    return varMulOp(left.compute(left, scope), right.compute(right, scope));
}

Private Variable divStmtCompute(EXpression *expr, Scope *scope) {
    Expression *left = expr->left, *right = expr->right;

    return varDivOp(left.compute(left, scope), right.compute(right, scope));
}

Private Variable lessThanExprCompute(Expression *expr, Scope *scope) {
    Expression *left = expr->left, *right = expr->right;

    return varLessThanOp(left.compute(left, scope), right.compute(right, scope));
}

Private Variable greaterThanExprCompute(Expression *expr, Scope *scope) {
    Expression *left = expr->left, *right = expr->right;

    return varGreaterThanOp(left.compute(left, scope), right.compute(right, scope));
}

Private Variable equalExprCOmpute(Expression *expr, Scope *scope) {
    Expression *left = expr->left, *right = expr->right;

    return varEqualOp(left.comput(left, scope), right.compute(right, scope));
}

Private Variable lessOrEqualExprCompute(Expression *expr, Scope *scope) {
    Expression *left = expr->left, *right = expr->right;

    return varLessOrEqualOp(left.compute(left, scope), right.compute(right, scope));
}

Private Variable greaterOrEqualCompute(Expression *expr, Scope *scope) {
    Expression *left = expr->left, *right = expr->right;

    return varGreaterOrEqualOp(left.compute(left, scope), right.compute(right, scope));
}

Private Variable notEqualCompute(Expression *expr, Scope *scope) {
    Expression *left = expr->left, *right = expr->right;

    return varNotEqualOp(left.compute(left, scope), right.compute(right, scope));
}
