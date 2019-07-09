/* epression.c */

#include "statement.h"
#include "expression.h"
#include "wrapper.h"
#include "string.h"
#include "case.h"

/* private prototypes */
private Variable * constExprCompute(Expression *, Scope *);
private Variable * assignExprCompute(Expression *, Scope *);
private Variable * memberSelectExprCompute(Expression *, Scope *);
private Variable * funcCallExprCompute(Expression *, Scope *);
private Variable * identExprCompute(Expression *expr, Scope *scope);

// Operators
private Variable * plusExprCompute(Expression *expr, Scope *scope);
private Variable * minusExprCompute(Expression *expr, Scope *scope);
private Variable * mulExprCompute(Expression *expr, Scope *scope);
private Variable * divExprCompute(Expression *expr, Scope *scope);
private Variable * lessThanExprCompute(Expression *expr, Scope *scope);
private Variable * greaterThanExprCompute(Expression *expr, Scope *scope);
private Variable * equalExprCompute(Expression *expr, Scope *scope);
private Variable * lessOrEqualExprCompute(Expression *expr, Scope *scope);
private Variable * greaterOrEqualCompute(Expression *expr, Scope *scope);
private Variable * notEqualCompute(Expression *expr, Scope *scope);

/* Public procedures */

// Constant expression
ConstantExpression * constExprDefault() {
    ConstantExpression *expr = (ConstantExpression *)zMalloc(sizeof(ConstantExpression));
    expr->base.compute = constExprCompute;

    return expr;
}

_Status_t constExprSetInt(ConstantExpression *expr, int num) {
    Primitive *int_pri = primitiveGen(&num, PRIMITIVE_TYPE_INT);
    Variable *var = varGen(NULL, VAR_PRIMITIVE_INT, int_pri);

    expr->constant_var = var;

    return OK;
}

_Status_t constExprSetStr(ConstantExpression *expr, char *str) {
    Primitive *str_pri = primitiveGen(str, PRIMITIVE_TYPE_STR);
    Variable *var = varGen(NULL, VAR_PRIMITIVE_STR, str_pri);

    expr->constant_var = var;

    return OK;
}

void constExprRelease(ConstantExpression *expr, Scope *scope) {
    ConstantExpression *cExpr = (ConstantExpression *)expr;

    varRelease(cExpr->constant_var);
    free(expr);
}

// Asignment expression
AssignmentExpression * assignExprDefault() {
    AssignmentExpression *aExpr = (AssignmentExpression *)zMalloc(sizeof(AssignmentExpression));
    aExpr->base.compute = assignExprCompute;

    return aExpr;
}

AssignmentExpression * assignExprGen(Expression *left, Expression *right) {
    AssignmentExpression *aExpr = assignExprDefault();
    ASSIGN_SET_LEFT(aExpr, left);
    ASSIGN_SET_RIGHT(aExpr, right);

    return aExpr;
}

void assignExprRelease(Expression *expr, Scope *scope) {
    AssignmentExpression *aExpr = (AssignmentExpression *)expr;

    exprRelease(ASSIGN_GET_LEFT(aExpr), scope);
    exprRelease(ASSIGN_GET_RIGHT(aExpr), scope);
}

// Member select expression
MemberSelectExpression * memberSelectDefault() {
    MemberSelectExpression *mExpr = (MemberSelectExpression *)zMalloc(sizeof(MemberSelectExpression));
    mExpr->base.compute = memberSelectExprCompute;

    return mExpr;
}

void memberSelectRelease(Expression *expr, Scope *scope) {
    MemberSelectExpression *mExpr = (MemberSelectExpression *)expr;

    Expression *head = mExpr->head;
    if (head) head->release(head, scope);
    if (mExpr->member) free(mExpr->member);
}

// Function call expression
FuncCallExpression * funcCallExprDefault() {
    FuncCallExpression *f = (FuncCallExpression *)zMalloc(sizeof(FuncCallExpression));
    f->base.compute = funcCallExprCompute;
    return f;
}

FuncCallExpression * funcCallExprGen(char *ident, list *arguments) {
    FuncCallExpression *f = funcCallExprDefault();
    f->funcIdent = ident;
    f->arguments = arguments;

    return f;
}

void funcCallRelease(Expression *expr, Scope *s) {
    FuncCallExpression *fExpr = (FuncCallExpression *)expr;

    free(fExpr->funcIdent);

    listRelease(fExpr->arguments);
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

void identExprRelease(Expression *expr, Scope *s) {
    IdentExpression *iExpr = (IdentExpression *)expr;

    free(iExpr->ident);
}

// Plus expression
PlusExpression * plusExprDefault() {
    PlusExpression *expr = (PlusExpression *)zMalloc(sizeof(PlusExpression));
    expr->base.compute = plusExprCompute;
    return expr;
}

PlusExpression * plusExprGen(Expression *left, Expression *right) {
    PlusExpression *expr = (PlusExpression *)plusExprDefault();
    PLUS_EXPR_SET_LEFT(expr, left);
    PLUS_EXPR_SET_RIGHT(expr, right);

    return expr;
}

void plusExprRelease(Expression *expr, Scope *s) {
    PlusExpression *pExpr = (PlusExpression *)expr;

    Expression *left = pExpr->left, *right = pExpr->right;

    left->release(left, s);
    right->release(right, s);
}

// Minus expression
MinusExpression * minusExprDefault() {
    MinusExpression *expr = (MinusExpression *)zMalloc(sizeof(MinusExpression));
    expr->base.compute = minusExprCompute;
    return expr;
}

MinusExpression * minusExprGen(Expression *left, Expression *right) {
    MinusExpression *expr = minusExprDefault();
    MINUS_EXPR_SET_LEFT(expr, left);
    MINUS_EXPR_SET_RIGHT(expr, right);
    return expr;
}

void minusExprRelease(Expression *expr, Scope *s) {
    MinusExpression *mExpr = (MinusExpression *)expr;

    Expression *left = mExpr->left, *right = mExpr->right;
    left->release(left, s);
    right->release(right, s);
}

// Mul expression
MulExpression * mulExprDefault() {
    MulExpression *mExpr = (MulExpression *)zMalloc(sizeof(MulExpression));
    mExpr->base.compute = mulExprCompute;
    return mExpr;
}

MulExpression * mulExprGen(Expression *left, Expression *right) {
    MulExpression *mExpr = mulExprDefault();
    MUL_EXPR_SET_LEFT(mExpr, left);
    MUL_EXPR_SET_RIGHT(mExpr, right);

    return mExpr;
}

void mulExprRelease(Expression *expr, Scope *s) {
    MulExpression *mExpr = (MulExpression *)expr;

    Expression *left = mExpr->left, *right = mExpr->right;
    left->release(left, s);
    right->release(right, s);
}

// Div expression
DivExpression * divExprDefault() {
    DivExpression *dExpr = (DivExpression *)zMalloc(sizeof(DivExpression));
    dExpr->base.compute = divExprCompute;
    return dExpr;
}

DivExpression * divExprGen(Expression *l, Expression *r) {
    DivExpression *dExpr = (DivExpression *)divExprDefault();
    MUL_EXPR_SET_LEFT(dExpr, l);
    MUL_EXPR_SET_RIGHT(dExpr, r);
    return dExpr;
}

void divExprRelease(Expression *expr, Scope *s) {
    DivExpression *dExpr = (DivExpression *)expr;

    Expression *left = dExpr->left, *right = dExpr->right;
    left->release(left, s);
    right->release(right, s);
}

// Less thant expression
LessThanExpression * lessThanExprDefault() {
    LessThanExpression *lExpr = (LessThanExpression *)zMalloc(sizeof(LessThanExpression));
    lExpr->base.compute = lessThanExprCompute;
    return lExpr;
}

LessThanExpression * lessThanExprGen(Expression *left, Expression *right) {
    LessThanExpression *lExpr = lessThanExprDefault();
    LESS_THAN_SET_LEFT(lExpr, left);
    LESS_THAN_SET_RIGHT(lExpr, right);

    return lExpr;
}

void lessThanExprRelease(Expression *expr, Scope *s) {
    LessThanExpression * lExpr = (LessThanExpression *)expr;

    Expression *left = lExpr->left, *right = lExpr->right;
    left->release(left, s);
    right->release(right, s);
}

// Greater than expression
GreaterThanExpression * greaterThanExprDefault() {
    GreaterThanExpression *gExpr = (GreaterThanExpression *)zMalloc(sizeof(GreaterThanExpression));
    gExpr->base.compute = greaterThanExprCompute;
    return gExpr;
}

GreaterThanExpression * greaterThanExprGen(Expression *left, Expression *right) {
    GreaterThanExpression *gExpr = greaterThanExprDefault();
    GREATER_THAN_SET_LEFT(gExpr, left);
    GREATER_THAN_SET_RIGHT(gExpr, right);
    return gExpr;
}

void greaterThanExprRelease(Expression *expr, Scope *s) {
    GreaterThanExpression *gExpr = (GreaterThanExpression *)expr;

    Expression *left = gExpr->left, *right = gExpr->right;
    left->release(left, s);
    right->release(right, s);
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

void equalExprRelease(Expression *expr, Scope *s) {
    EqualExpression *eExpr = (EqualExpression *)expr;

    Expression *left = eExpr->left, *right = eExpr->right;
    left->release(left, s);
    right->release(right, s);
}

// Less or equal expression
LessOrEqualExpression * lessOrEqualExprDefault() {
    LessOrEqualExpression *expr = (LessOrEqualExpression *)zMalloc(sizeof(LessOrEqualExpression));
    expr->base.compute = lessOrEqualExprCompute;
    return expr;
}

LessOrEqualExpression * lessOrEqualExprGen(Expression *left, Expression *right) {
    LessOrEqualExpression *expr = (LessOrEqualExpression *)lessOrEqualExprDefault();
    LESS_OR_EQUAL_EXPR_SET_LEFT(expr, left);
    LESS_OR_EQUAL_EXPR_SET_RIGHT(expr, right);

    return expr;
}

void lessOrEqualExprRelease(Expression *expr, Scope *s) {
    LessOrEqualExpression *lExpr = (LessOrEqualExpression *)expr;

    Expression *left = lExpr->left, *right = lExpr->right;
    left->release(left, s);
    right->release(right, s);
}

// Greater or equal expression
GreaterOrEqualExpression * greaterOrEqualExprDefault() {
    GreaterOrEqualExpression *expr = (GreaterOrEqualExpression *)zMalloc(sizeof(GreaterOrEqualExpression));
    expr->base.compute = greaterOrEqualCompute;
    return expr;
}

GreaterOrEqualExpression *greaterOrEqualGen(Expression *left, Expression *right) {
    GreaterOrEqualExpression *expr = (GreaterOrEqualExpression *)greaterOrEqualExprDefault();
    GREATER_OR_EQUAL_EXPR_SET_LEFT(expr, left);
    GREATER_OR_EQUAL_EXPR_SET_RIGHT(expr, right);

    return expr;
}

void greaterOrEqualRelease(Expression *expr, Scope *s) {
    GreaterOrEqualExpression *gExpr = (GreaterOrEqualExpression *)expr;

    Expression *left = gExpr->left, *right = gExpr->right;
    left->release(left, s);
    right->release(right, s);
}

// Not Equal expression
NotEqualExpression * notEqualExprDefault() {
    NotEqualExpression *expr = (NotEqualExpression *)zMalloc(sizeof(NotEqualExpression));
    expr->base.compute = notEqualCompute;
    return expr;
}

NotEqualExpression * notEqualExprGen(Expression *left, Expression *right) {
    NotEqualExpression *expr = notEqualExprDefault();

    NOT_EQUAL_EXPR_SET_LEFT(expr, left);
    NOT_EQUAL_EXPR_SET_RIGHT(expr, right);

    return expr;
}

void notEqualExprRelease(Expression *expr, Scope *s) {
    NotEqualExpression *nExpr = (NotEqualExpression *)expr;

    Expression *left = nExpr->left, *right = nExpr->right;

    left->release(left, s);
    right->release(right, s);
}

/* private procedures */
private Variable * constExprCompute(Expression *expr, Scope *scope) {
    ConstantExpression *cExpr = (ConstantExpression *)expr;

    return CONSTANT_GET_VAR(cExpr);
}

// Operators
private Variable * identExprCompute(Expression *expr, Scope *scope) {
    IdentExpression *iExpr = (IdentExpression *)expr;

    char *ident = iExpr->ident;

    if (scope->primitives) {
        Variable *p = scopeGetPrimitive(scope, ident);
        if (p) return p;
    }

    if (scope->objects) {
        Variable *o = scopeGetObject(scope, ident);
        if (o) return o;
    }

    // Can not find the identifer in the current scope.
    return NULL;
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

private Variable * memberSelectExprCompute(Expression *expr, Scope *scope) {
    MemberSelectExpression *mExpr = (MemberSelectExpression *)expr;

    Expression *head = mExpr->head;
    Variable *var = head->compute(head, scope);

    if (!VAR_IS_OBJECT(var)) return NULL;

    return VAR_BIN_OP_CALL(var, dot, var, (void *)mExpr->member);
}

private Variable * funcCallExprCompute(Expression *expr, Scope *scope) {
    FuncCallExpression *fExpr = (FuncCallExpression *)expr;

    /* Generate a local scope */
    Scope *subScope = subScopeGenerate(scope);

    Func *f = scopeGetFunc(subScope, FUNC_CALL_IDENT(fExpr));

    /* Argument check */
    list *parameters = f->parameters;

    list *arguments = FUNC_CALL_ARGUMENT(fExpr);

    /* Parameters is without exists which means parameters is empty */
    if (parameters == NULL) {
        /* Arguments list should be null too */
        if (arguments == NULL)
            goto CALL_TO_PROC;
        else {
            scopeRelease(subScope);
            return NULL;
        }
    }

    listIter par_iter = listGetIter(parameters, LITER_FORWARD),
        arg_iter = listGetIter(arguments, LITER_FORWARD);

    listNode *par, *arg;

    while (true) {
        par = listNext(&par_iter);
        arg = listNext(&arg_iter);

        if (par == NULL || arg == NULL) break;

        pair *par_pair = par->value, *arg_pair = arg->value;

        if (strCompare(PAIR_GET_RIGHT(par_pair), PAIR_GET_RIGHT(arg_pair))) {
            /* Match */
            continue;
        } else {
            /* Mismatch */
            scopeRelease(subScope);
            return null;
        }
    }

    /* Mismatch cause number of parameters and arguments is different. */
    if (par != arg) return NULL;

    /* Push arguments into local scope */
    listRewind(&par_iter);
    listRewind(&arg_iter);

    while (true) {
        par = listNext(&par_iter);
        arg = listNext(&arg_iter);

        if (par == NULL || arg == NULL) break;

        pair *par_pair = par->value, *arg_pair = arg->value;

        Expression *expr = PAIR_GET_LEFT(arg_pair);
        Variable *argVar = expr->compute(expr, subScope);

        pair *argPair_;
        if (VAR_IS_PRIMITIVE(argVar)) {
            argPair_ = pairGen(PAIR_GET_LEFT(par_pair), argVar->p, NULL, NULL, NULL);
            scopeNewPrimitive(subScope, argPair_);
        } else if (VAR_IS_OBJECT(argVar)) {
            argPair_ = pairGen(PAIR_GET_LEFT(par_pair), argVar->o, NULL, NULL, NULL);
            scopeNewObject(subScope, argPair_);
        }
    }

    /* First to check is call to function is defined in internal module
     * fixme: function call to internal module */
CALL_TO_PROC:
    /* Then search function in upper scope */
    if (isNull(f)) {
        scopeRelease(subScope);
        return NULL;
    }

    scopeRelease(subScope);
    return f->compute(f, subScope);
}

private Variable * assignExprCompute(Expression *expr, Scope *scope) {
    AssignmentExpression *aExpr = (AssignmentExpression *)expr;

    pair operands = VAR_EXTRACT_FROM_EXPR(aExpr->l, aExpr->r, scope);

    return VAR_BIN_OP_CALL((Variable *)PAIR_GET_LEFT(&operands), assign,
                           PAIR_GET_LEFT(&operands),
                           PAIR_GET_RIGHT(&operands));
}

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

private Variable * equalExprCompute(Expression *expr, Scope *scope) {
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

#ifdef _AST_TREE_TESTING_

#include "test.h"

void identTest(void);
void memberSelectTest(void);
void funcCallTest(void);

void exprTest(void **state) {
    ConstantExpression *operand_l = constExprDefault(),
        *operand_r = constExprDefault();

    constExprSetInt(operand_l, 2);
    constExprSetInt(operand_r, 1);

    // Plus
    Expression *plusExpr = (Expression *)plusExprGen((Expression *)operand_l, (Expression *)operand_r);
    Variable *sum = plusExpr->compute(plusExpr, null);

    assert_int_equal(getPrimitive_int(sum->p), 3);
    varRelease(sum);

    // Minus
    Expression *minusExpr = (Expression *)minusExprGen((Expression *)operand_l, (Expression *)operand_r);
    Variable *diff = minusExpr->compute(minusExpr, null);

    assert_int_equal(getPrimitive_int(diff->p), 1);
    varRelease(diff);

    // 1 + (2 - 1)
    Expression *hybrid = (Expression *)plusExprGen((Expression *)operand_l, minusExpr);
    Variable *v3 = hybrid->compute(hybrid, null);

    assert_int_equal(getPrimitive_int(v3->p), 3);
    varRelease(v3);

    // Multiplication
    Expression *mulExpr = (Expression *)mulExprGen((Expression *)operand_l, (Expression *)operand_r);
    Variable *product = mulExpr->compute(mulExpr, null);

    assert_int_equal(getPrimitive_int(product->p), 2);
    varRelease(product);

    // Division
    Expression *divExpr = (Expression *)divExprGen((Expression *)operand_l, (Expression *)operand_r);
    Variable *quotient = divExpr->compute(divExpr, null);

    assert_int_equal(getPrimitive_int(quotient->p), 2);
    varRelease(quotient);

    // Less than
    Expression *lessThanExpr = (Expression *)lessThanExprGen((Expression *)operand_l, (Expression *)operand_r);
    Variable *isLess = lessThanExpr->compute(lessThanExpr, null);

    assert_int_equal(getPrimitive_int(isLess->p), 0);
    varRelease(isLess);

    // Greater than
    Expression *greaterThanExpr = (Expression *)greaterThanExprGen((Expression *)operand_l, (Expression *)operand_r);
    Variable *isGreater = greaterThanExpr->compute(greaterThanExpr, null);

    assert_int_equal(getPrimitive_int(isGreater->p), 1);
    varRelease(isGreater);

    // Equal
    Expression *equalExpr = (Expression *)equalExprGen((Expression *)operand_l, (Expression *)operand_r);
    Variable *isEqual = equalExpr->compute(equalExpr, null);

    assert_int_equal(getPrimitive_int(isEqual->p), 0);
    varRelease(isEqual);

    // Less or equal
    Expression *lessOrEqual = (Expression *)lessOrEqualExprGen((Expression *)operand_l, (Expression *)operand_r);
    Variable *isLessOrEqual = lessOrEqual->compute(lessOrEqual, null);

    assert_int_equal(getPrimitive_int(isLessOrEqual->p), 0);
    varRelease(isLessOrEqual);

    // Greater or equal
    Expression *greaterOrEqual = (Expression *)greaterOrEqualGen((Expression *)operand_l, (Expression *)operand_r);
    Variable *isGreaterOrEqual = greaterOrEqual->compute(greaterOrEqual, null);

    assert_int_equal(getPrimitive_int(isGreaterOrEqual->p), 1);
    varRelease(isGreaterOrEqual);

    // Not equal
    Expression *notEqual = (Expression *)notEqualExprGen((Expression *)operand_l, (Expression *)operand_r);
    Variable *isNotEqual = notEqual->compute(notEqual, null);

    assert_int_equal(getPrimitive_int(isNotEqual->p), 1);
    varRelease(isNotEqual);

    // Identifier
    identTest();

    // Member
    memberSelectTest();

    // Function call
    funcCallTest();
}

void identTest(void) {
    Scope *scope = scopeGenerate();

    int a = 0;
    Primitive *pri = primitiveGen(&a, PRIMITIVE_TYPE_INT);

    Variable *pri_var = varGen(strdup("INT_VAR"), VAR_PRIMITIVE_INT, pri);
    scopeNewPrimitive(scope, pairGen(strdup("INT_VAR"), pri_var, NULL, NULL, NULL));

    Expression *iExpr = (Expression *)identExprGen("INT_VAR");
    Variable *v = iExpr->compute(iExpr, scope);

    assert_non_null(v);
    assert_int_equal(getPrimitive_int(v->p), 0);
}

void memberSelectTest(void) {
    Scope *scope = scopeGenerate();
    MemberSelectExpression *mExpr = memberSelectDefault();

    int a = 0;
    Object *obj = objGen(strdup("AA"), strdup("AType"));
    Variable *member_pri = varGen("aa", VAR_PRIMITIVE_INT, primitiveGen(&a, PRIMITIVE_TYPE_INT));

    list *members = listCreate();

    listSetDupMethod(members, varDup);
    listSetMatchMethod(members, varIdentCmp);
    listSetReleaseMethod(members, varRelease);

    listAppend(members, member_pri);

    OBJ_SET_MEMBERS(obj, members);

    scopeNewObject(scope, pairGen(strdup("AA"), varGen(strdup("AA"), VAR_OBJECT, obj), NULL, NULL, NULL));

    MEMBER_SELECT_SET_SUBS(mExpr, "aa");
    MEMBER_SELECT_SET_HEAD(mExpr, (Expression *)identExprGen("AA"));

    Variable *v = ((Expression *)mExpr)->compute((Expression *)mExpr, scope);
    assert_non_null(v);
    assert_int_equal(getPrimitive_int(v->p), 0);
}

void funcCallTest(void) {
    Scope *scope = scopeGenerate();

    /* Without arguments */
    FuncCallExpression *fExpr = funcCallExprGen(strdup("f"), NULL);

    Func *f_def = funcGenerate();

    Expression *left = (Expression *)constExprDefault(),
        *right = (Expression *)constExprDefault();
    constExprSetInt((ConstantExpression *)left, 1);
    constExprSetInt((ConstantExpression *)right, 2);

    Statement *stmt = (ReturnStatement *)returnStmtGen(plusExprGen(left, right));

    funcAppendStatements(f_def, stmt);

    Expression *base = (Expression *)fExpr;
    base->compute(base, scope);
}

#endif /* _AST_TREE_TESTING_ */
