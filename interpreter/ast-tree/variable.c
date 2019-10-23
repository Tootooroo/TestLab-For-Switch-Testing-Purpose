/* variable.c */

#include "variable.h"
#include "wrapper.h"
#include "pair.h"
#include "string.h"
#include "object.h"
#include "array.h"

#include "variable_ops.h"

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

private VarInitRtn initRtns[VAR_TYPE_NUM] = {};

/* Private prototypes */
private _Bool varOpDefSpaceCheck_Binary(Variable *l, Variable *r, VarOp op);
private _Bool varSupportCheck(Variable *, VarOp);


// Inner Operations
private Variable * varPrimitivePass(Variable *l, Scope *s, char *ident);

/* Private variables */
VarInnerOps primitiveInnerOps = {
    varPrimitivePass
};

/* Public procedures */
VarInitRtn getVarInitRtn(VarType t) {
    return initRtns[t];
}
void setVarInitRtn(VarType t, VarInitRtn r) {
    initRtns[t] = r;
}

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

    VarInitRtn rtn = getVarInitRtn(type);
    rtn(var, ident, value);

    return var;
}

void varRelease(Variable *v) {
    if (v->identifier) free(v->identifier);

    VAR_INNER_OP(v, valueDestruct, v);

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

    dup->o = VAR_INNER_OP(orig, valueDup, orig);

    return dup;
}

void varReset(Variable *l) {
    if (l->identifier) free(l->identifier);

    VAR_INNER_OP(l, valueDestruct, l);
}

_Status_t varAssign_(Variable *l, Variable *r) {
    /* Clear l's content */
    varReset(l);

    /* Assign from r to l */
    l->iOps = r->iOps;
    l->ops = r->ops;
    l->type = r->type;

    if (r->identifier)
        l->identifier = strdup(r->identifier);

    l->o = VAR_INNER_OP(l, valueDup, r);

    return OK;
}

// Misc
_Bool varIdentCmp(Variable *v, char *ident) {
    return strCompare(VAR_IDENT(v), ident);
}

_Bool varTypeCmp(Variable *v1, Variable *v2) {

    if (VAR_IS_OBJECT(v1)) {
        return objTypeCmp(v1->o, v2->o);
    } else {
        if (v1->type != v2->type)
            return false;
        else
            return true;
    }

    return false;
}

_Bool varIsType(Variable *v, char *type) {

    if (VAR_IS_OBJECT(v)) {
        return strCompare(v->o->objectType, type);
    } else if (VAR_IS_PRIMITIVE(v)) {
        return strCompare(varPriTypesTable[varTypeIs(v)], type);
    }
    return false;
}

_Bool varIsTrue(Variable *var) {
    if (varTypeIs(var) == VAR_PRIMITIVE_INT) {
        if (getPrimitive_int(var->p) == 0 )
            return false;
        else
            return true;
    } else if (varTypeIs(var) == VAR_PRIMITIVE_STR) {
        if (getPrimitive_str(var->p) == NULL)
            return false;
        else
            return true;
    } else {
        abortWithMsg("Require boolean value");
    }
}

VarType varTypeStr2Int(char *typeStr) {
    int current = VAR_PRIMITIVE_INT;

    while (current < VAR_OBJECT) {
        _Bool isMatch = strCompare(varPriTypesTable[current], typeStr);
        if (isMatch) return current;
    }
    return VAR_EMPTY;
}

_Bool varOpsPreCheck(Variable *l, Variable *r, VarOp ops, VarType type) {
    return varOpDefSpaceCheck_Binary(l, r, ops) == false ||
        varTypeIs(l) != type;
}

private Variable * varPrimitivePass(Variable *l, Scope *s, char *ident) {
    Variable *dup = varDup(l);
    VAR_SET_IDENT(dup, ident);

    pair *p = pairGen(strdup(ident), l);
    scopeNewPrimitive(s, p);

    return dup;
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

VarType varTypeIs(Variable *v) {
    return v->type;
}

char * varType2Str(Variable *v) {}

#ifdef _AST_TREE_TESTING_

#include "test.h"


#endif /* _AST_TREE_TESTING_ */
