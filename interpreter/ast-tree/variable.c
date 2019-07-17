/* variable.c */

#include "variable.h"
#include "wrapper.h"
#include "pair.h"
#include "string.h"
#include "object.h"

/* Private Variables */
int opOnTypes[VAR_TYPE_NUM][VAR_OP_NUM] = {
    /* +, -, *, /, ==, <, >, <=, >=, !=, =, .  */
    {  1, 1, 1, 1,  1, 1, 1,  1,  1,  1, 1, 1 }, /* Integer */
    {  0, 0, 0, 0,  0, 0, 0,  0,  0,  0, 0, 0 }, /* String */
    {  0, 0, 0, 0,  0, 0, 0,  0,  0,  0, 0, 0 }, /* OPS */
};

char *varPriTypesTable[VAR_OBJECT] = {
    "Int",
    "String",
    "Ops"
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
private Variable * varAssign(Variable *l, Variable *r);
private Variable * varDot(Variable *l, Variable *r);

// Inner Operations
private Variable * varPrimitivePass(Variable *l, Scope *s, char *ident);

/* Private variables */
private VarInnerOps primitiveInnerOps = {
    varPrimitivePass
};

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
    varNotEqualOp,
    /* Assign */
    varAssign,
    /* Dot */
    NULL,
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
    /* Assign */
    NULL,
    /* Dot */
    NULL
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
    /* Assign */
    NULL,
    /* Dot */
    varDot
};

/* Public procedures */
Variable varDefault_Empty() {
    Variable var = {
        .identifier = null,
        .type = VAR_EMPTY,
        .o = null,
        .ops = null,
        .iOps = null
    };

    return var;
}

Variable * varDefault() {
    Variable *var = (Variable *)zMalloc(sizeof(Variable));

    var->identifier = null;
    var->type = VAR_EMPTY;
    var->o = null;
    var->ops = null;
    var->iOps = null;

    return var;
}

Variable * varGen(char *ident, VarType type, void *value) {
    Variable *var = varDefault();
    var->identifier = ident;
    var->type = type;

    if (type == VAR_PRIMITIVE_INT) {
        var->p = (Primitive *)value;
        var->ops = &primitiveOps_integer;
        var->iOps = &primitiveInnerOps;
    } else if (type == VAR_PRIMITIVE_STR) {
        var->p = (Primitive *)value;
        var->ops = &primitiveOps_string;
        var->iOps = &primitiveInnerOps;
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

    // Variable's identifier maybe null
    if (VAR_IDENT(orig))
        VAR_SET_IDENT(dup, strdup(VAR_IDENT(orig)));

    VAR_SET_TYPE(dup, VAR_TYPE(orig));

    dup->ops = orig->ops;
    dup->iOps = orig->iOps;

    if (VAR_IS_PRIMITIVE(orig)) {
        /* Primitive */
        VAR_SET_PRIMITIVE(dup, primitiveDup(orig->p));
    } else {
        /* Object */
        VAR_SET_OBJECT(dup, objDup(orig->o));
    }
    return dup;
}

_Status_t varAssign_(Variable *l, Variable *r) {
    /* Clear l's content */
    l->iOps = null;
    l->ops = null;

    if (l->type == VAR_OBJECT) {
        objectRelease(l->o);
    } else {
        /* Primitive */
        primitiveRelease(l->p);
    }

    if (l->identifier) free(l->identifier);

    /* Assign from r to l */
    l->iOps = r->iOps;
    l->ops = r->ops;
    l->type = r->type;
    if (r->identifier)
        l->identifier = strdup(r->identifier);
    else
        l->identifier = null;

    if (r->type == VAR_OBJECT) {
        l->o = objDup(r->o);
    } else {
        l->p = primitiveDup(r->p);
    }

    return OK;
}

// Misc
_Bool varIdentCmp(Variable *v, char *ident) {
    return strCompare(VAR_IDENT(v), ident);
}

_Bool varTypeCmp(Variable *v1, Variable *v2) {
    if (VAR_IS_PRIMITIVE(v1)) {
        if (v1->type != v2->type)
            return false;
        else
            return true;
    } else if (VAR_IS_OBJECT(v1)) {
        return objTypeCmp(v1->o, v2->o);
    }

    return false;
}

_Bool varIsType(Variable *v, char *type) {
    if (VAR_IS_PRIMITIVE(v)) {
        return strCompare(varPriTypesTable[varTypeIs(v)], type);
    } else if (VAR_IS_OBJECT(v)) {
        return strCompare(v->o->objectType, type);
    }

    return false;
}

/* Private procedures */

/* Operators */

/* Operatos of integer */

/* Variable * (*)(Variable *, Variable *, VarOp, VarType) */
#define VAR_OPS_INT_PRE_CHECK(l, r, ops, type)\
    varOpDefSpaceCheck_Binary(l, r, ops) == false ||\
    varTypeIs(l) != type

/* Variable * (*)(Variable *, Variable *, VarType) */
#define VAR_OPS_BINARY_INT_COMPUTE(V1, V2, OP) ({\
    Primitive *l_pri = l->p, *r_pri = r->p, *ret_pri;\
    int ret_int = l_pri->val_i OP r_pri->val_i;\
    ret_pri = primitiveGen(&ret_int, PRIMITIVE_TYPE_INT);  \
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

/* fixme: variable assign should also change value of variable l */
private Variable * varAssign(Variable *l, Variable *r) {
    // Only left-value can be assigned.
    if (!VAR_IS_LVAL(l)) return null;
    if (VAR_OPS_INT_PRE_CHECK(l, r, VAR_OP_ASSIGN, VAR_PRIMITIVE_INT))
        return NULL;

    return VAR_OPS_BINARY_INT_COMPUTE(l, r, =);
}

private Variable * varPrimitivePass(Variable *l, Scope *s, char *ident) {
    Variable *dup = varDup(l);
    VAR_SET_IDENT(dup, ident);

    pair *p = pairGen(strdup(ident), l);
    scopeNewPrimitive(s, p);

    return dup;
}

/* Requires(Object(l), String(r)) */
private Variable * varDot(Variable *l, Variable *r) {
    char *member = (char *)r;

    if (!VAR_IS_OBJECT(l)) return NULL;

    return objGetMember(l->o, member);
}

/* Operators of string */

/* Operators of ops */
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

private void varTest_str(void);
private void varTest_int(void);

void varTest(void **state) {
    varTest_int();
}

private void varTest_str(void) {

}

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
