/* variable.c */

#include "variable.h"
#include "wrapper.h"
#include "pair.h"
#include "string.h"
#include "object.h"

/* Private Variables */
int opOnTypes[VAR_TYPE_NUM][VAR_OP_NUM] = {
    /* +, -, *, /, ==, <, >, <=, >=, !=  */
    {  1, 1, 1, 1,  1, 1, 1,  1,  1,  1 }, /* Integer */
    {  0, 0, 0, 0,  0, 0, 0,  0,  0,  0 }, /* String */
    {  0, 0, 0, 0,  0, 0, 0,  0,  0,  0 }, /* OPS */
};

/* Private prototypes */
private _Bool varOpDefSpaceCheck_Binary(Variable *l, Variable *r, VarOp op);
private _Bool varSupportCheck(Variable *, VarOp);
private VarType varTypeIs(Variable *v);

// Operators
private Variable * varPlusOp(Variable *l, Variable *r);
private Variable * varMinusOp(Variable *l, Variable *r);
private Variable * varMulOp(Variable *l, Variable *r);
private Variable * varDivOp(Variable *l, Variable *r);
private Variable * varEqualOp(Variable *l, Variable *r);
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
    /* Equal */
    varEqualOp,
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
    /* Equal */
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
    /* Equal */
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

void varRelease(Variable *v) {
    if (v->identifier) free(v->identifier);
    if (VAR_IS_PRIMITIVE(v)) {
        primitiveRelease(v->p);
    } else {
        objectRelease(v->o);
    }

    free(v);
}

Variable * varDup(Variable *orig) {
    Variable *dup = varDefault();

    VAR_SET_IDENT(dup, strdup(VAR_IDENT(orig)));
    VAR_SET_TYPE(dup, VAR_TYPE(orig));

    if (VAR_IS_PRIMITIVE(orig)) {
        /* Primitive */
        VAR_SET_PRIMITIVE(dup, primitiveDup(orig->p));
    } else {
        /* Object */
        VAR_SET_OBJECT(dup, objDup(orig->o));
    }
}

/* Private procedures */

/* Operators */

/* Operatos of integer */
#define VAR_OPS_INT_PRE_CHECK(l, r, ops, type)\
    varOpDefSpaceCheck_Binary(l, r, ops) == false ||\
    varTypeIs(l) != type

#define VAR_OPS_BINARY_INT_COMPUTE(V1, V2, OP) ({\
    Primitive *l_pri = l->p, *r_pri = r->p, *ret_pri;\
    int ret_int = l_pri->val_i OP r_pri->val_i;\
    ret_pri = primitiveGenerate_i(ret_int);\
    varGen(NULL, VAR_PRIMITIVE_INT, ret_pri);\
})

private Variable * varPlusOp(Variable *l, Variable *r) {

    if (VAR_OPS_INT_PRE_CHECK(l, r, VAR_OP_PLUS, VAR_PRIMITIVE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, +);
}

private Variable * varMinusOp(Variable *l, Variable *r) {

    if (VAR_OPS_INT_PRE_CHECK(l, r, VAR_OP_MINUS, VAR_PRIMITIVE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, -);
}

private Variable * varMulOp(Variable *l, Variable *r) {

    if (VAR_OPS_INT_PRE_CHECK(l, r, VAR_OP_MUL, VAR_PRIMITIVE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, *);
}

private Variable * varDivOp(Variable *l, Variable *r) {

    if (VAR_OPS_INT_PRE_CHECK(l, r, VAR_OP_DIV, VAR_PRIMITIVE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, /);
}

private Variable *varEqualOp(Variable *l, Variable *r) {
    if (VAR_OPS_INT_PRE_CHECK(l, r, VAR_OP_EQUAL, VAR_PRIMITIVE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, ==);
}

private Variable * varLessThanOp(Variable *l, Variable *r) {

    if (VAR_OPS_INT_PRE_CHECK(l, r, VAR_OP_LESS_THAN, VAR_PRIMITIVE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, <);
}

private Variable * varGreaterThanOp(Variable *l, Variable *r) {

    if (VAR_OPS_INT_PRE_CHECK(l, r, VAR_OP_GREATER_THAN, VAR_PRIMITIVE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, >);
}

private Variable * varLessOrEqualOp(Variable *l, Variable *r) {

    if (VAR_OPS_INT_PRE_CHECK(l, r, VAR_OP_LESS_OR_EQUAL, VAR_PRIMITIVE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, <=);
}

private Variable * varGreaterOrEqualOp(Variable *l, Variable *r) {

    if (VAR_OPS_INT_PRE_CHECK(l, r, VAR_OP_GREATER_OR_EQUAL, VAR_PRIMITIVE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, >=);
}

private Variable * varNotEqualOp(Variable *l, Variable *r) {

    if (VAR_OPS_INT_PRE_CHECK(l, r, VAR_OP_NOT_EQUAL, VAR_PRIMITIVE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, !=);
}

private _Bool varOpDefSpaceCheck_Binary(Variable *l, Variable *r, VarOp op) {
    VarType left_type = varTypeIs(l), right_type = varTypeIs(r);

    if (left_type != right_type) return false;
    // To check whether the type of variable
    // support <op> operation.
    if (!varSupportCheck(l, op)) return false;

    return true;
}

/* fixme: implement pending */
private _Bool varSupportCheck(Variable *v, VarOp op) {
    VarType type = varTypeIs(v);
    return opOnTypes[type][op];
}

private VarType  varTypeIs(Variable *v) {
    return v->type;
}

#ifdef _AST_TREE_TESTING_

#include "test.h"

void varTest(void *state) {
    int left_int = 2, right_int = 1;;
    Variable *left = varGen(NULL, VAR_PRIMITIVE_INT, primitiveGenerate_i(left_int));
    Variable *right = varGen(NULL, VAR_PRIMITIVE_INT, primitiveGenerate_i(right_int));

    Variable *sum = varPlusOp(left, right);
    assert_non_null(sum);
    assert_int_equal(getPrimitive_int(sum->p), 3);
}

#endif /* _AST_TREE_TESTING_ */
