/* epression.c */

#include "statement.h"
#include "expression.h"
#include "wrapper.h"
#include "string.h"
#include "func.h"
#include "parameter.h"

#include "program.h"

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
    exprSetType(expr, EXPR_TYPE_CONSTANT);

    return expr;
}

ConstantExpression * constExprGen(void *value, primitiveType type) {
    ConstantExpression *expr = constExprDefault();

    switch (type) {
    case PRIMITIVE_TYPE_INT:
        expr->constant_var = varGen(NULL, VAR_PRIMITIVE_INT, value);
        break;
    case PRIMITIVE_TYPE_STR:
        expr->constant_var = varGen(NULL, VAR_PRIMITIVE_STR, value);
        break;
    case PRIMITIVE_TYPE_OPS:
        break;
    default:
        break;
    }
    return expr;
}

_Status_t constExprSetInt(ConstantExpression *expr, int num) {
    Variable *var = varGen(NULL, VAR_PRIMITIVE_INT, &num);

    expr->constant_var = var;

    return OK;
}

_Status_t constExprSetStr(ConstantExpression *expr, char *str) {
    Variable *var = varGen(NULL, VAR_PRIMITIVE_STR, str);

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
    exprSetType(aExpr, EXPR_TYPE_ASSIGN);

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
    exprSetType(mExpr, EXPR_TYPE_MEMBER_REF);

    return mExpr;
}

MemberSelectExpression * memberSelectGen(Expression *objName, char *member) {
    MemberSelectExpression *mExpr = memberSelectDefault();

    MEMBER_SELECT_SET_HEAD(mExpr, objName);
    MEMBER_SELECT_SET_SUBS(mExpr, member);

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
    exprSetType(f, EXPR_TYPE_FUNC_CALL);

    return f;
}

FuncCallExpression * funcCallExprGen(char *ident, Arguments *arguments) {
    FuncCallExpression *f = funcCallExprDefault();
    f->funcIdent = ident;
    if (arguments)
        f->args = arguments;
    else
        f->args = argusGen();

    return f;
}

void funcCallRelease(Expression *expr, Scope *s) {
    FuncCallExpression *fExpr = (FuncCallExpression *)expr;

    free(fExpr->funcIdent);
    argusRelease(fExpr->args);
}

_Status_t funcCallAddArg(Expression *expr, Argument *arg) {
    FuncCallExpression *fExpr = (FuncCallExpression *)expr;

    argusAdd(fExpr->args, arg);

    return OK;
}

// Identifier expression
IdentExpression * identExprDefault() {
    IdentExpression *iExpr = (IdentExpression *)zMalloc(sizeof(IdentExpression));

    iExpr->base.compute = identExprCompute;
    exprSetType(iExpr, EXPR_TYPE_IDENTIFIER);

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
    exprSetType(lExpr, EXPR_TYPE_ORDER);
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
    exprSetType(gExpr, EXPR_TYPE_ORDER);
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
    exprSetType(expr, EXPR_TYPE_ORDER);

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
    exprSetType(expr, EXPR_TYPE_ORDER);
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
    exprSetType(expr, EXPR_TYPE_ORDER);
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
    exprSetType(expr, EXPR_TYPE_ORDER);
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

    Variable *p = scopeGetPrimitive(scope, ident);
    if (p) return p;

    Variable *o = scopeGetObject(scope, ident);
    if (o) return o;

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

    /* Find definition of funtion in current or upper scope; */
    Func *f = scopeGetFunc(scope, FUNC_CALL_IDENT(fExpr));
    if (isNull(f)) return NULL;

    /* Internal procedure's outer pointer may not be initialized,
     * due to the global scope doesn't be generate when moduleInit callled */
    if (!f->outer && FUNC_IS_INTERNAL(f))
        f->outer = global_scope;

    /* Generate a local scope */
    Scope *subScope = subScopeGenerate(f->outer);
    f->current = scope;

    Parameters *parameters = f->params;
    Arguments *arguments = FUNC_CALL_ARGUMENT(fExpr);

    /* Deal with arguments */
    argusStore(arguments, parameters, subScope);

    /* First to check is call to function is defined in internal module */
    Variable *ret = null;
    if (FUNC_IS_INTERNAL(f)) ret = f->interRtn(f, subScope);
    else {
        /* Computing */
        ret = f->compute(f, subScope);
    }

    // Return type checking
    if (varIsType(ret, FUNC_RETURN_TYPE(f)) == false) {
        return null;
    }

    scopeReleaseCurrent(subScope);

    return ret;
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
    scopeNewPrimitive(scope, pairGen_M(strdup("INT_VAR"), pri_var, NULL, NULL, NULL));

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

    scopeNewObject(scope, pairGen_M(strdup("AA"), varGen(strdup("AA"), VAR_OBJECT, obj), NULL, NULL, NULL));

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

    Func *f_def = funcGen(strdup("f"), strdup("Int"), NULL, NULL);

    int a = 1, b = 2;
    Statement *stmt = (Statement *)returnStmtGen(plusExprGen(constExprGen(&a, PRIMITIVE_TYPE_INT),
                                                             constExprGen(&b, PRIMITIVE_TYPE_INT)));
    funcAppendStatements(f_def, stmt);
    scopeNewFunc(scope, pairGen(strdup("f"), f_def));

    Expression *base = (Expression *)fExpr;
    Variable *v = base->compute(base, scope);

    assert_non_null(v);
    assert_int_equal(getPrimitive_int(v->p), 3);

    /* With arguments */
    Scope *s_arg = scopeGenerate();

    // Define function
    Func *f_def_arg = funcGen(strdup("f_arg"), strdup("Int"), NULL, NULL);
    funcAddParam(f_def_arg, paramGen(strdup("a"), strdup("Int")));

    int a_ = 1;
    stmt = (Statement *)returnStmtGen(plusExprGen(identExprGen(strdup("a_int")),
                                                  constExprGen(&a_, PRIMITIVE_TYPE_INT)));
    funcAppendStatements(f_def_arg, stmt);
    scopeNewFunc(s_arg, pairGen(strdup("f_arg"), f_def_arg));

    int a_int = 1;
    Variable *var_ = varGen(strdup("a_int"), VAR_PRIMITIVE_INT, &a_int);

    scopeNewPrimitive(f_def_arg->outer, pairGen(strdup("a_int"), var_));

    // Function call expression
    FuncCallExpression *fExpr_arg = funcCallExprGen(strdup("f_arg"), NULL);
    funcCallAddArg((Expression *)fExpr_arg, arguGen(identExprGen(strdup("a_int"))));

    base = (Expression *)fExpr_arg;
    v = base->compute(base, s_arg);

    assert_non_null(v);
    assert_int_equal(getPrimitive_int(v->p), 2);

}

#endif /* _AST_TREE_TESTING_ */
