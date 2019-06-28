/* variable.c */

#include "variable.h"
#include "pair.h"

/* Private prototypes */
Private _Bool varOpDefSpaceCheck_Binary(Variable *l, Variable *r);
Private _Bool varSupportCheck(Variable *, VarOp);
Private pair * varTypeIs(Variable *v);

/* Public procedures */
Variable varDefault_Empty() {
    Variable var = {
        .identifier = null,
        .type = VAR_EMPTY,
        .o = null;
    };

    return var;
}

Variable * varDefault() {
    Variable *var = (Variable *)zMalloc(sizeof(Variable));
    *var = varDefault_Empty();

    return var;
}

Variable * varGen(char *ident, VarType type, void *value) {
    Variable *var = varDefault();
    var->identifier = ident;
    var->type = type;

    if (type == VAR_PRIMITIVE) {
        var->p = (Primitive *)value;
    } else if (type == VAR_OBJECT) {
        var->o = (Object *)value;
    }
    return var;
}

/* Operators */
/* l and r should be same type */
Variable * varPlusOp(Variable *l, Variable *r) {
    if (varOpDefSapceCheck_Binary(l, r, VAR_OP_PLUS) == false)
        return NULL;

    // Generate a temporary variable, a right value
    return varGen(NULL, VAR_PRIMITIVE, primitivePlusOp(l->p, r->p));
}

Variable * varMinusOp(Variable *l, Variable *r) {
    if (varOpDefSpaceCheck_Binary(l, r, VAR_OP_MINUS) == false)
        return NULL;

    // Generate a temporary variable, a right value
    return varGen(NULL, VAR_PRIMITIVE, primitiveMinusOp(l->p, r->p));
}

Variable * varMulOp(Variable *l, Variable *r) {
    if (varOpDefSapceCheck_Binary(l, r, VAR_OP_MUL) == false)
        return NULL;

    // Generate a temporary variable, a right value
    return varGen(NULL, VAR_PRIMITIVE, primitiveMulOp(l->p, r->p));
}

Variable * varDivOp(Variable *l, Variable *r) {
    if (varOpDefSpaceCheck_Binary(l, r, VAR_OP_DIV) == false)
        return NULL;

    // Generate a temporary variable, a right value
    return varGen(NULL, VAR_PRIMITIVE, primitiveDivOp(l->p, r->p));
}

Variable * varLessThanOp(Variable *l, Variable *r) {
    if (varOpDefSpaceCheck_Binary(l, r, VAR_OP_LESS_THAN) == false)
        return NULL;

    // Generate a temporary variable, a right value
    return varGen(NULL, VAR_PRIMITIVE, primitiveLessThanOp(l->p, r->p));
}

Variable * varGreaterThanOp(Variable *l, Variable *r) {
    if (varOpDefSpaceCheck_Binary(l, r, VAR_OP_GREATER_THAN) == false)
        return NULL;

    // Generate a temporary variable, a right value
    return varGen(NULL, VAR_PRIMITIVE, primitiveGreaterThanOp(l->p, r->p));
}

Variable * varLessOrEqualOp(Variable *l, Variable *r) {
    if (varOpDefSpaceCheck_Binary(l, r, VAR_OP_LESS_OR_EQUAL) == false)
        return NULL;

    // Generate a temporary variable, a right value
    return varGen(NULL, VAR_PRIMITIVE, primitiveLessOrEqualOp(l->p, r->p));
}

/* Private procedures */

/* fixme: How to implement ? */
Private _Bool varOpDefSpaceCheck_Binary(Variable *l, Variable *r, VarOp op) {
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
Private _Bool varSupportCheck(Variable *, VarOp) {
    return true;
}

Private pair * varTypeIs(Variable *v) {
    VarType t = v->type;
    primitiveType pType = v->p->type;

    pair *p = pairGenerate((void *)t, (void *)pType);
    return p;
}
