/* epression.c */

#include "expression.h"
#include "wrapper.h"

/* private prototypes */
private Variable * constExprCompute(Expression *, Scope *);
private Variable * assignExprCompute(Expression *, Scope *);
private Variable * memberSelectExprCompute(Expression *, Scope *);
private Variable * funcCallExprCompute(Expression *, Scope *);
private Variable * identExprCompute(Expression *expr, Scope *scope);

// Operators
private Variable  * plusExprCompute(Expression *expr, Scope *scope);
private Variable  * minusStmtCompute(Expression *expr, Scope *scope);
private Variable  * mulStmtCompute(Expression *expr, Scope *scope);
private Variable  * divStmtCompute(Expression *expr, Scope *scope);
private Variable  * lessThanExprCompute(Expression *expr, Scope *scope);
private Variable  * greaterThanExprCompute(Expression *expr, Scope *scope);
private Variable  * equalExprCOmpute(Expression *expr, Scope *scope);
private Variable  * lessOrEqualExprCompute(Expression *expr, Scope *scope);
private Variable  * greaterOrEqualCompute(Expression *expr, Scope *scope);
private Variable  * notEqualCompute(Expression *expr, Scope *scope);

/* Public procedures */

// Constant expression
ConstantExpression * constExprDefault() {
    ConstantExpression *expr = (ConstantExpression *)zMalloc(sizeof(ConstantExpression));
    expr->base.compute = constExprCompute;

    return expr;
}

_Status_t constExprSetInt(ConstantExpression *expr, int num) {
    Primitive *int_pri = primitiveGenerate_i(num);
    Variable *var = varGen(NULL, VAR_PRIMITIVE_INT, int_pri);

    expr->constant_var = var;

    return OK;
}

_Status_t constExprSetStr(ConstantExpression *expr, char *str) {
    Primitive *str_pri = primitiveGenerate_s(str);
    Variable *var = varGen(NULL, VAR_PRIMITIVE_STR, str_pri);

    expr->constant_var = var;

    return OK;
}

void constExprRelease(ConstantExpression *expr, Scope *scope) {
    ConstantExpression *cExpr = (ConstantExpression *)expr;

}

// Asignment expression
AssignmentExpression * assignExprDefault() {
    AssignmentExpression *aExpr = (AssignmentExpression *)zMalloc(sizeof(AssignmentExpression));
    aExpr->base.compute = assignExprCompute;

    return aExpr;
}

AssignmentExpression * assignExprGen(Expression *left, Expression *right) {
    AssignmentExpression *aExpr = assignExprDefault();
    aExpr->l = left;
    aExpr->r = right;

    return aExpr;
}

void assignExprRelease(Expression *expr, Scope *scope) {
    AssignmentExpression *aExpr = (AssignmentExpression *)expr;

    Expression *left = aExpr->l, *right  = aExpr->r;

    exprRelease(left, scope);
    exprRelease(right, scope)
}

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
PlusExpression * plusExprDefault() {
    PlusExpression *expr = (PlusExpression *)zMalloc(sizeof(PlusExpression));
    expr->base.compute = plusExprCompute;
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

/* private procedures */
private Variable * constExprCompute(Expression *expr, Scope *scope) {
    ConstantExpression *cExpr = (ConstantExpression *)expr;

    return CONSTANT_GET_VAR(cExpr);
}

private Variable * assignExprCompute(Expression *expr, Scope *scope) {}

private Variable * memberSelectExprCompute(Expression *expr, Scope *scope) {}

private Variable * funcCallExprCompute(Expression *expr, Scope *scope) {}

// Operators
private Variable * identExprCompute(Expression *expr, Scope *scope) {
    IdentExpression *iExpr = expr;

    char *ident = iExpr->ident;

    Primitive *p = scopeGetPrimitive(scope, ident);
    if (p) return varGen(strdup(ident), VAR_PRIMITIVE, p);

    Object *o = scopeGetObject(scope, ident);
    if (o) return varGen(strdup(ident), VAR_OBJECT, o);

    // Can not find the identifer in the current scope.
    return varDefault_Empty();
}

#define VAR_EXTRACT_FROM_EXPR(LEFT_EXPR, RIGHT_EXPR, SCOPE) ({ \
    pair ret;\
    Variable *left_var = exprCompute(LEFT_EXPR, SCOPE);\
    Variable *right_var = exprCompute(RIGHT_EXPR, SCOPE);\
    \
    PAIR_SET_LEFT(&ret, (void *)left_var);\
    PAIR_SET_RIGHT(&ret, (void *)right_var);\
    \
    ret;\
})

private Variable * plusExprCompute(Expression *expr, Scope *scope) {
    PlusExpression *pExpr = (PlusExpression *)expr;

    pair operands = VAR_EXTRACT_FROM_EXPR(pExpr->left, pExpr->right, scope);

    return VAR_BIN_OP_CALL((Variable *)PAIR_GET_LEFT(&operands), plus,
                           PAIR_GET_LEFT(&operands),
                           PAIR_GET_RIGHT(&operands));
}

private Variable * minusExprCompute(Expression *expr, Scope *scope) {
    MinusExpression *mExpr = (MinusExpression *)expr;

    pair operands = VAR_EXTRACT_FROM_EXPR(mExpr->left, mExpr->right, scope);

    return VAR_BIN_OP_CALL((Variable *)PAIR_GET_LEFT(&operands), minus,
                           PAIR_GET_LEFT(&operands),
                           PAIR_GET_RIGHT(&operands));
}

private Variable * mulExprCompute(Expression *expr, Scope *scope) {
    MulExpression *mExpr = (MulExpression *)expr;

    pair operands = VAR_EXTRACT_FROM_EXPR(mExpr->left, mExpr->right, scope);

    return VAR_BIN_OP_CALL((Variable *)PAIR_GET_LEFT(&operands), mul,
                           PAIR_GET_LEFT(&operands),
                           PAIR_GET_RIGHT(&operands));
}

private Variable * divExprCompute(Expression *expr, Scope *scope) {
    DivExpression *dExpr = (DivExpression *)expr;

    pair operands = VAR_EXTRACT_FROM_EXPR(dExpr->left, dExpr->right, scope);

    return VAR_BIN_OP_CALL((Variable *)PAIR_GET_LEFT(&operands), div,
                           PAIR_GET_LEFT(&operands),
                           PAIR_GET_RIGHT(&operands));
}

private Variable * lessThanExprCompute(Expression *expr, Scope *scope) {
    LessThanExpression *lExpr = (LessThanExpression *)expr;

    pair operands = VAR_EXTRACT_FROM_EXPR(lExpr->left, lExpr->right, scope);

    return VAR_BIN_OP_CALL((Variable *)PAIR_GET_LEFT(&operands), lessThan,
                           PAIR_GET_LEFT(&operands),
                           PAIR_GET_RIGHT(&operands));
}

private Variable * greaterThanExprCompute(Expression *expr, Scope *scope) {
    GreaterThanExpression *gExpr = (GreaterThanExpression *)expr;

    pair operands = VAR_EXTRACT_FROM_EXPR(gExpr->left, gExpr->right, scope);

    return VAR_BIN_OP_CALL((Variable *)PAIR_GET_LEFT(&operands), greaterThan,
                           PAIR_GET_LEFT(&operands),
                           PAIR_GET_RIGHT(&operands));
}

private Variable * equalExprCOmpute(Expression *expr, Scope *scope) {
    EqualExpression *eExpr = (EqualExpression *)expr;

    pair operands = VAR_EXTRACT_FROM_EXPR(eExpr->left, eExpr->right, scope);

    return VAR_BIN_OP_CALL((Variable *)PAIR_GET_LEFT(&operands), equal,
                           PAIR_GET_LEFT(&operands),
                           PAIR_GET_RIGHT(&operands));
}

private Variable * lessOrEqualExprCompute(Expression *expr, Scope *scope) {
    LessOrEqualExpression *lExpr = (LessOrEqualExpression *)expr;

    pair operands = VAR_EXTRACT_FROM_EXPR(lExpr->left, lExpr->right, scope);

    return VAR_BIN_OP_CALL((Variable *)PAIR_GET_LEFT(&operands), lessOrEqual,
                           PAIR_GET_LEFT(&operands),
                           PAIR_GET_RIGHT(&operands));
}

private Variable * greaterOrEqualCompute(Expression *expr, Scope *scope) {
    GreaterOrEqualExpression *gExpr = (GreaterOrEqualExpression *)expr;

    pair operands = VAR_EXTRACT_FROM_EXPR(gExpr->left, gExpr->right, scope);

    return VAR_BIN_OP_CALL((Variable *)PAIR_GET_LEFT(&operands), greaterOrEqual,
                           PAIR_GET_LEFT(&operands),
                           PAIR_GET_RIGHT(&operands));
}

private Variable * notEqualCompute(Expression *expr, Scope *scope) {
    NotEqualExpression *nExpr = (NotEqualExpression *)expr;

    pair operands = VAR_EXTRACT_FROM_EXPR(nExpr->left, nExpr->right, scope);

    return VAR_BIN_OP_CALL((Variable *)PAIR_GET_LEFT(&operands), notEqual,
                           PAIR_GET_LEFT(&operands),
                           PAIR_GET_RIGHT(&operands));
}
