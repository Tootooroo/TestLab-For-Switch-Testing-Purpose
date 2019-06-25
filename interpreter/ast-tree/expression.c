/* epression.c */

#include "expression.h"

/* Private prototypes */
Variable * constExprCompute(Expression *, Scope *);

/* Public procedures */
ConstantExpression * constExprDefault() {
    ConstantExpression *expr = (ConstantExpression *)zMalloc(sizeof(ConstantExpression));
    expr->base.compute = constExprCompute;

    return expr;
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
