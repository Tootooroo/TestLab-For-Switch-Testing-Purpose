/* variable.c */

#include "variable.h"
#include "wrapper.h"
#include "pair.h"

/* Private prototypes */
private _Bool varOpDefSpaceCheck_Binary(Variable *l, Variable *r, VarOp op);
private _Bool varSupportCheck(Variable *, VarOp);
private pair * varTypeIs(Variable *v);

// Operators
private Variable * varPlusOp(Variable *l, Variable *r);
private Variable * varMinusOp(Variable *l, Variable *r);
private Variable * varMulOp(Variable *l, Variable *r);
private Variable * varDivOp(Variable *l, Variable *r);
private Variable * varLessThanOp(Variable *l, Variable *r);
private Variable * varGreaterThanOp(Variable *l, Variable *r);
private Variable * varLessOrEqualOp(Variable *l, Variable *r);
private Variable * varGreaterOrEqualOp(Variable *l, Variable *r);
private Variable * varNotEqualOp(Variable *l, Variable *r);

/* Private variables */
private VarOps primitiveOps_integer = {
    /* Plus */
    varPlusOp,
    /* Minus */
    varMinusOp,
    /* Multiplication */
    varMulOp,
    /* Division */
    varDivOp,
    /* Less than */
    varLessThanOp,
    /* Greater than */
    varGreaterThanOp,
    /* Less or equal */
    varLessOrEqualOp,
    /* Greater or equal */
    varGreaterOrEqualOp,
    /* Not euqal */
    varNotEqualOp
};

/* fixme: give defined to operators of string type */
private VarOps primitiveOps_string = {
    /* Plus */
    NULL,
    /* Minus */
    NULL,
    /* Multiplication */
    NULL,
    /* Division */
    NULL,
    /* Less than */
    NULL,
    /* Greater than */
    NULL,
    /* Less or equal */
    NULL,
    /* Greater or equal */
    NULL,
    /* Not euqal */
    NULL,
};

/* fixme: give defined to operators of object type */
private VarOps objectOps = {
    /* Plus */
    NULL,
    /* Minus */
    NULL,
    /* Multiplication */
    NULL,
    /* Division */
    NULL,
    /* Less than */
    NULL,
    /* Greater than */
    NULL,
    /* Less or equal */
    NULL,
    /* Greater or equal */
    NULL,
    /* Not euqal */
    NULL,
};

/* Public procedures */
Variable varDefault_Empty() {
    Variable var = {
        .identifier = null,
        .type = VAR_EMPTY,
        .o = null,
        .ops = null
    };

    return var;
}

Variable * varDefault() {
    Variable *var = (Variable *)zMalloc(sizeof(Variable));

    var->identifier = null;
    var->type = VAR_EMPTY;
    var->o = null;
    var->ops = null;

    return var;
}

Variable * varGen(char *ident, VarType type, void *value) {
    Variable *var = varDefault();
    var->identifier = ident;
    var->type = type;

    if (type == VAR_PRIMITIVE_INT) {
        var->p = (Primitive *)value;
        var->ops = &primitiveOps_integer;
    } else if (type == VAR_PRIMITIVE_STR) {
        var->p = (Primitive *)value;
        var->ops = &primitiveOps_string;
    } else if (type == VAR_OBJECT) {
        var->o = (Object *)value;
        var->ops = &objectOps;
    }
    return var;
}

/* Private procedures */

/* Operators */

/* Operatos of integer */
#define VAR_OPS_INT_PRE_CHECK(l, r, ops, type)\
    varOpDefSpaceCheck_Binary(l, r, ops) == false &&\
    varTypeIs(l)->right_i != type

#define VAR_OPS_BINARY_INT_COMPUTE(V1, V2, OP) ({\
    Primitive *l_pri = l->p, *r_pri = r->p, *ret_pri;\
    int ret_int = l_pri->val_i OP r_pri->val_i;\
    ret_pri = primitiveGenerate_i(ret_int);\
    varGen(NULL, VAR_PRIMITIVE_INT, ret_pri);\
})

private Variable * varPlusOp(Variable *l, Variable *r) {

    if (VAR_OPS_INT_PRE_CHECK(l, r, VAR_OP_PLUS, PRIMITIVE_TYPE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, +);
}

private Variable * varMinusOp(Variable *l, Variable *r) {

    if (VAR_OPS_INT_PRE_CHECK(l, r, VAR_OP_PLUS, PRIMITIVE_TYPE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, -);
}

private Variable * varMulOp(Variable *l, Variable *r) {

    if (VAR_OPS_INT_PRE_CHECK(l, r, VAR_OP_PLUS, PRIMITIVE_TYPE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, *);
}

private Variable * varDivOp(Variable *l, Variable *r) {

    if (VAR_OPS_INT_PRE_CHECK(l, r, VAR_OP_PLUS, PRIMITIVE_TYPE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, /);
}

private Variable * varLessThanOp(Variable *l, Variable *r) {

    if (VAR_OPS_INT_PRE_CHECK(l, r, VAR_OP_PLUS, PRIMITIVE_TYPE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, <);
}

private Variable * varGreaterThanOp(Variable *l, Variable *r) {

    if (VAR_OPS_INT_PRE_CHECK(l, r, VAR_OP_PLUS, PRIMITIVE_TYPE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, >);
}

private Variable * varLessOrEqualOp(Variable *l, Variable *r) {

    if (VAR_OPS_INT_PRE_CHECK(l, r, VAR_OP_PLUS, PRIMITIVE_TYPE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, <=);
}

private Variable * varGreaterOrEqualOp(Variable *l, Variable *r) {

    if (VAR_OPS_INT_PRE_CHECK(l, r, VAR_OP_PLUS, PRIMITIVE_TYPE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, >=);
}

private Variable * varNotEqualOp(Variable *l, Variable *r) {

    if (VAR_OPS_INT_PRE_CHECK(l, r, VAR_OP_PLUS, PRIMITIVE_TYPE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, !=);
}

private _Bool varOpDefSpaceCheck_Binary(Variable *l, Variable *r, VarOp op) {
    pair *left_type = varTypeIs(l), *right_type = varTypeIs(r);

    _Bool isSame = pairLeft_i(left_type) == pairLeft_i(right_type) &&
        pairRight_i(left_type) == pairRight_i(right_type);

    if (!isSame) return false;
    // To check whether the type of variable
    // support <op> operation.
    if (!varSupportCheck(l, op)) return false;

    return true;
}

/* fixme: implement pending */
private _Bool varSupportCheck(Variable *v, VarOp op) {
    return true;
}

private pair * varTypeIs(Variable *v) {
    VarType t = v->type;
    primitiveType pType = v->p->type;

    return pairGen_Integer(t, pType, null, null, null);
}
