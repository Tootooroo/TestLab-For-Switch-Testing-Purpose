/* file: variable_ops.c
 *
 * This file is divided into several part, the number of parts is equal to
 * number of type defined here.
 *
 * Table of Contents:
 * ( Search the keyword @SECX (X is the number correspond to the specified section)
 *   to iterative overall section )
 *
 * 1.SEC1 -- Integer
 * 2.SEC2 -- Object
 * 3.SEC3 -- Array
 * 4.SEC4 -- String
 * */

#include "string.h"
#include "variable_ops.h"

#define OPERATION_NOT_SUPPORT NULL
#define NO_OPERATION_DEFINED NULL

/* Private Prototypes */
private void varIntInit(Variable *v, char *ident, void *value);
private void varStrInit(Variable *v, char *ident, void *value);
private void varObjInitRtn(Variable *v, char *ident, void *value);
void varArrayInitRtn(Variable *v, char *ident, void *value);

/* Variable operation module init */
void variableOpsInit(void) {
    // Int InitRtn Setup
    setVarInitRtn(VAR_PRIMITIVE_INT, varIntInit);
    // String InitRtn setup
    setVarInitRtn(VAR_PRIMITIVE_STR, varStrInit);
    // Object InitRtn setup
    setVarInitRtn(VAR_OBJECT, varObjInitRtn);
    // Array InitRtn setup
    setVarInitRtn(VAR_ARRAY, varArrayInitRtn);
}

/* @SEC1 -- Integer */

// Operations
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
private Variable * varAssign(Variable *l, Variable *r);
private Variable * varPrimitivePass(Variable *l, Scope *s, char *ident);

// Inner operations
private void * varIntValueDup(Variable *v);
_Status_t varPrimitiveDestruct(Variable *v);

// Integer Ops
VarOps primitiveOps_integer = {
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
    varNotEqualOp,
    /* Assign */
    varAssign,
    /* Dot */
    NO_OPERATION_DEFINED,
};

VarInnerOps primitiveInnerOps_integer = {
    /* pass */
    varPrimitivePass,
    /* dup */
    NO_OPERATION_DEFINED,
    /* valueDup */
    varIntValueDup,
    /* Value Destruct */
    varPrimitiveDestruct
};

/* Procedure to initialize variable of
 * Integer while you're calling varGen. */
private void varIntInit(Variable *v, char *ident, void *value) {
    v->p = primitiveGen(value, PRIMITIVE_TYPE_INT);
    v->ops = &primitiveOps_integer;
    v->iOps = &primitiveInnerOps_integer;
}

/* Inner Operations of integer variable */
private Variable * varPrimitivePass(Variable *l, Scope *s, char *ident) {
    Variable *dup = varDup(l);
    VAR_SET_IDENT(dup, ident);

    pair *p = pairGen(strdup(ident), l);
    scopeNewPrimitive(s, p);

    return dup;
}

private void * varIntValueDup(Variable *v) {
    return primitiveDup(v->p);
}

_Status_t varPrimitiveDestruct(Variable *v) {
    if (v->p)
        primitiveRelease(v->p);
    return OK;
}

/* Operators of integer */

/* Variable * (*)(Variable *, Variable *, VarType) */
#define VAR_OPS_BINARY_INT_COMPUTE(V1, V2, OP) ({\
    Primitive *l_pri = V1->p, *r_pri = V2->p;\
    int ret_int = l_pri->val_i OP r_pri->val_i;\
    varGen(NULL, VAR_PRIMITIVE_INT, &ret_int);\
})

private Variable * varPlusOp(Variable *l, Variable *r) {

    if (varOpsPreCheck(l, r, VAR_OP_PLUS, VAR_PRIMITIVE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, +);
}

private Variable * varMinusOp(Variable *l, Variable *r) {

    if (varOpsPreCheck(l, r, VAR_OP_MINUS, VAR_PRIMITIVE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, -);
}

private Variable * varMulOp(Variable *l, Variable *r) {

    if (varOpsPreCheck(l, r, VAR_OP_MUL, VAR_PRIMITIVE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, *);
}

private Variable * varDivOp(Variable *l, Variable *r) {

    if (varOpsPreCheck(l, r, VAR_OP_DIV, VAR_PRIMITIVE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, /);
}

private Variable *varEqualOp(Variable *l, Variable *r) {
    if (varOpsPreCheck(l, r, VAR_OP_EQUAL, VAR_PRIMITIVE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, ==);
}

private Variable * varLessThanOp(Variable *l, Variable *r) {
    if (varOpsPreCheck(l, r, VAR_OP_LESS_THAN, VAR_PRIMITIVE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, <);
}

private Variable * varGreaterThanOp(Variable *l, Variable *r) {

    if (varOpsPreCheck(l, r, VAR_OP_GREATER_THAN, VAR_PRIMITIVE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, >);
}

private Variable * varLessOrEqualOp(Variable *l, Variable *r) {

    if (varOpsPreCheck(l, r, VAR_OP_LESS_OR_EQUAL, VAR_PRIMITIVE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, <=);
}

private Variable * varGreaterOrEqualOp(Variable *l, Variable *r) {

    if (varOpsPreCheck(l, r, VAR_OP_GREATER_OR_EQUAL, VAR_PRIMITIVE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, >=);
}

private Variable * varNotEqualOp(Variable *l, Variable *r) {

    if (varOpsPreCheck(l, r, VAR_OP_NOT_EQUAL, VAR_PRIMITIVE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, !=);
}

/* fixme: variable assign should also change value of variable l */
private Variable * varAssign(Variable *l, Variable *r) {
    // Only left-value can be assigned.
    if (!VAR_IS_LVAL(l)) return null;
    if (varOpsPreCheck(l, r, VAR_OP_ASSIGN, VAR_PRIMITIVE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, =);
}

#ifdef _AST_TREE_TESTING_

#include "test.h"

private void varTest_str(void);
private void varTest_int(void);

void varTest(void **state) {
    varTest_int();
}

private void varTest_str(void) {}

private void varTest_int(void) {
    int left_int = 2, right_int = 1;;
    Variable *left = varGen(NULL, VAR_PRIMITIVE_INT, primitiveGen(&left_int, PRIMITIVE_TYPE_INT));
    Variable *right = varGen(NULL, VAR_PRIMITIVE_INT, primitiveGen(&right_int, PRIMITIVE_TYPE_INT));

    /* Plus */
    Variable *sum = varPlusOp(left, right);
    assert_non_null(sum);
    assert_int_equal(getPrimitive_int(sum->p), 3);
    varRelease(sum);

    /* Minus */
    Variable *diff = varMinusOp(left, right);
    assert_non_null(diff);
    assert_int_equal(getPrimitive_int(diff->p), 1);
    varRelease(diff);

    /* Multiplication */
    Variable *product = varMulOp(left, right);
    assert_non_null(product);
    assert_int_equal(getPrimitive_int(product->p), 2);
    varRelease(product);

    /* Division */
    Variable *quotient = varDivOp(left, right);
    assert_non_null(quotient);
    assert_int_equal(getPrimitive_int(quotient->p), 2);
    varRelease(quotient);

    /* Equal */
    Variable *isEqual = varEqualOp(left, right);
    assert_non_null(isEqual);
    assert_int_equal(getPrimitive_int(isEqual->p), 0);
    varRelease(isEqual);

    /* Less than */
    Variable *lessThan = varLessThanOp(left, right);
    assert_non_null(lessThan);
    assert_int_equal(getPrimitive_int(lessThan->p), 0);
    varRelease(lessThan);

    /* Greater than */
    Variable *greaterThan = varGreaterThanOp(left, right);
    assert_non_null(greaterThan);
    assert_int_equal(getPrimitive_int(greaterThan->p), 1);
    varRelease(greaterThan);

    /* Less or equal */
    Variable *lessOrEqual = varLessOrEqualOp(left, right);
    assert_non_null(lessOrEqual);
    assert_int_equal(getPrimitive_int(lessOrEqual->p), 0);
    varRelease(lessOrEqual);

    /* Greater or equal */
    Variable *greaterOrEqual = varGreaterOrEqualOp(left, right);
    assert_non_null(greaterOrEqual);
    assert_int_equal(getPrimitive_int(greaterOrEqual->p), 1);
    varRelease(greaterOrEqual);

    /* Not Equal */
    Variable *notEqual = varNotEqualOp(left, right);
    assert_non_null(notEqual);
    assert_int_equal(getPrimitive_int(notEqual->p), 1);
    varRelease(notEqual);
}


#endif /* _AST_TREE_TESTING_ */


/* @SEC2 -- Object */

/* Operations */
private Variable * varDot(Variable *l, Variable *r);
private Variable * varObjectAssign(Variable *l, Variable *r);
private _Status_t objValDestruct(Variable *v);

/* Inner Operations */
private void * objValDup(Variable *v);

VarOps objectOps = {
    /* Plus */
    NO_OPERATION_DEFINED,
    /* Minus */
    NO_OPERATION_DEFINED,
    /* Multiplication */
    NO_OPERATION_DEFINED,
    /* Division */
    NO_OPERATION_DEFINED,
    /* Equal */
    NO_OPERATION_DEFINED,
    /* Less than */
    NO_OPERATION_DEFINED,
    /* Greater than */
    NO_OPERATION_DEFINED,
    /* Less or equal */
    NO_OPERATION_DEFINED,
    /* Greater or equal */
    NO_OPERATION_DEFINED,
    /* Not euqal */
    NO_OPERATION_DEFINED,
    /* Assign */
    varObjectAssign,
    /* Dot */
    varDot
};

VarInnerOps objectIOps = {
    /* Pass */
    NULL,
    /* Duplication */
    NULL,
    /* Value Dup */
    objValDup,
    /* Value Destruct */
    objValDestruct
};

void varObjInitRtn(Variable *v, char *ident, void *value) {
    v->o = (Object *)value;
    v->ops = &objectOps;
    v->iOps = &objectIOps;
}

/* Inner Operations */
private void * objValDup(Variable *v) {
    return objDup(v->o);
}

private _Status_t objValDestruct(Variable *v) {
    if (v->o)
        objectRelease(v->o);
    return OK;
}

/* Operations */
private Variable * varAssign_Object(Variable *l, Variable *r) {}

/* Requires(Object(l), String(r)) */
private Variable * varObjectAssign(Variable *l, Variable *r) {
    varAssign_(l, r);
    return l;
}

private Variable * varDot(Variable *l, Variable *r) {
    char *member = (char *)r;

    if (!VAR_IS_OBJECT(l)) return NULL;

    return objGetMember(l->o, member);
}

/* @SEC3 -- Array */

#include "array.h"

/* Operations */
private Variable * arrayOpsAssign(Variable *l, Variable *r);

/* Inner Operations */
private void * arrayValueDup(Variable *v);
private _Status_t arrayValDestruct(Variable *v);

VarOps arrayOps = {
    /* Plus */
    NO_OPERATION_DEFINED,
    /* Minus */
    NO_OPERATION_DEFINED,
    /* Multiplication */
    NO_OPERATION_DEFINED,
    /* Division */
    NO_OPERATION_DEFINED,
    /* Equal */
    NO_OPERATION_DEFINED,
    /* Less than */
    NO_OPERATION_DEFINED,
    /* Greater than */
    NO_OPERATION_DEFINED,
    /* Less or equal */
    NO_OPERATION_DEFINED,
    /* Greater or equal */
    NO_OPERATION_DEFINED,
    /* Not euqal */
    NO_OPERATION_DEFINED,
    /* Assign */
    arrayOpsAssign,
    /* Dot */
    NO_OPERATION_DEFINED
};

VarInnerOps arrayIOps = {
    /* pass */
    NO_OPERATION_DEFINED,
    /* dup */
    NO_OPERATION_DEFINED,
    /* Value dup */
    arrayValueDup,
    /* Value Destruct */
    arrayValDestruct
};

void varArrayInitRtn(Variable *v, char *ident, void *value) {
    v->array = value;
    v->ops = &arrayOps;
    v->iOps = &arrayIOps;
}

/* Operations */
private Variable * arrayOpsAssign(Variable *l, Variable *r) {
    varAssign_(l, r);
    return l;
}

/* Inner Operations */
private void * arrayValueDup(Variable *v) {
    return arrayDup(v->array);
}

private _Status_t arrayValDestruct(Variable *v) {
    if (v->array)
        arrayRelease(v->array);
    return OK;
}

/* @SEC4 -- String */

/* Inner Operations */
private void * strPrimitiveValueDup(Variable *v);
private _Status_t strPrimitiveValueDestruct(Variable *v);

VarOps strOps = {
    /* Plus */
    NO_OPERATION_DEFINED,
    /* Minus */
    NO_OPERATION_DEFINED,
    /* Multiplication */
    NO_OPERATION_DEFINED,
    /* Division */
    NO_OPERATION_DEFINED,
    /* Equal */
    NO_OPERATION_DEFINED,
    /* Less than */
    NO_OPERATION_DEFINED,
    /* Greater than */
    NO_OPERATION_DEFINED,
    /* Less or equal */
    NO_OPERATION_DEFINED,
    /* Greater or equal */
    NO_OPERATION_DEFINED,
    /* Not euqal */
    NO_OPERATION_DEFINED,
    /* Assign */
    NO_OPERATION_DEFINED,
    /* Dot */
    NO_OPERATION_DEFINED
};

VarInnerOps strIOps = {
    /* Pass */
    varPrimitivePass,
    /* Dup */
    NO_OPERATION_DEFINED,
    /* Value dup */
    strPrimitiveValueDup,
    /* Value Destruct */
    strPrimitiveValueDestruct
};

private void varStrInit(Variable *v, char *ident, void *value) {
    v->p = primitiveGen(value, PRIMITIVE_TYPE_STR);
    v->ops = &strOps;
    v->iOps = &strIOps;
}

/* Inner Operations */
void * strPrimitiveValueDup(Variable *v) {
    return primitiveDup(v->p);
}

private _Status_t strPrimitiveValueDestruct(Variable *v) {
    if (v->p)
        primitiveRelease(v->p);
    return OK;
}
