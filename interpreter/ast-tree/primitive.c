/* primitive.c */

#include "primitive.h"
#include "wrapper.h"

/* Private prototypes */
Private Primitive * primitivePlusOp_Integer(Primitive *l, Primitive *r);
Private Primitive * primitiveMinusOp_Integer(Primitive *l, Primitive *r);
Private Primitive * primitiveMulOp_Integer(Primitive *l, Primitive *r);
Private Primitive * primitiveDivOp_Integer(Primitive *l, Primitive *r);
Private Primitive * primitiveLessThanOp_Integer(Primitive *l, Primitive *r);
Private Primitive * primitiveGreaterThanOp_Integer(Primitive *l, Primitive *r);
Private Primitive * primitiveLessOrEqualOp_Integer(Primitive *l, Primitive *r);
Private Primitive * primitiveGreaterOrEqualOp_Integer(Primitive *l, Primitive *r);


/* Public Procedures */
Primitive * primitiveGenerate_i(int i) {
    Primitive *p = (Primitive *)zMalloc(sizeof(Primitive));

    p->type = PRIMITIVE_TYPE_INT;
    p->val_i = i;

    return p;
}

Primitive * primitiveGenerate_s(char *s) {
    Primitive *p = (Primitive *)zMalloc(sizeof(Primitive));

    p->type = PRIMITIVE_TYPE_STR;
    p->val_str = s;

    return p;
}

Primitive * primitiveGenerate_op(Ops op) {
    Primitive *p = (Primitive *)zMalloc(sizeof(Primitive));

    p->type = PRIMITIVE_TYPE_OPS;
    p->ops = op;

    return p;
}

_Status_t primitiveRelease(Primitive *p) {
    if (isPrimitive_str(p))
        free(p->val_str);
    else if (isPrimitive_ops(p))
        listRelease(p->ops.ops);

    free(p);

    return OK;
}

/* Operators
 * Definition space check is already done by caller(Variable)
 * so arguments should all support operators defined here. */

Primitive * primitivePlusOp(Primitive *l, Primitive *r) {
    if (isPrimitive_int(l)) return primitivePlusOp_Integer(l, r);

    return NULL;
}

Primitive * primitiveMinusOp(Primitive *l, Primitive *r) {
    if (isPrimitive_int(l)) return primitiveMinusOp_Integer(l, r);

    return NULL;
}

Primitive * primitiveMulOp(Primitive *l, Primitive *r) {
    if (isPrimitive_int(l)) return primitiveMulOp_Integer(l, r);

    return NULL;
}

Primitive * primitiveDivOp(Primitive *l, Primitive *r) {
    if (isPrimitive_int(l)) return primitiveMulOp_Integer(l, r);
}

Primitive * primitiveLessThanOp(Primitive *l, Primitive *r) {
    if (isPrimitive_int(l)) return primitiveLessThanOp_Integer(l, r);
}

Primitive * primitiveGreaterThanOp(Primitive *l, Primitive *r) {
    if (isPrimitive_int(l)) return primitiveGreaterThanOp_Integer(l, r);
}

Primitive * primitiveLessOrEqualOp(Primitive *l, Primitive *r) {
    if (isPrimitive_int(l)) return primitiveLessOrEqualOp_Integer(l, r);
}

Primitive * primitiveGreaterOrEqualOp(Primitive *l, Primitive *r) {
    if (isPrimitive_int(l)) return primitiveGreaterOrEqualOp_Integer(l, r);
}

Primitive * primitiveNotEqualOp(Primitive *l, Primitive *r) {
    if (isPrimitive_int(l)) return primitiveNotEqualOp_Integer(l, r);
}

/* Private procedures */
Private Primitive * primitivePlusOp_Integer(Primitive *l, Primitive *r) {
    return primitiveGenerate_i(l->val_i + r->val_i);
}

Private Primitive * primitiveMinusOp_Integer(Primitive *l, Primitive *r) {
    return primitiveGenerate_i(l->val_i + r->val_i);
}

Private Primitive * primitiveMulOp_Integer(Primitive *l, Primitve *r) {
    return primitiveGenerate_i(l->val_i + r->val_i);
}

Private Primitive * primitiveDivOp_Integer(Primitive *l, Primitive *r) {
    int left = l->val_i, right = r->val_i;

    // Prevent the situation divided by 0
    if (right == 0) abortWithMsg("Error: Divided by 0");

    return primitiveGenerate_i(l->val_i / r->val_i);
}

Private Primitive * primitiveLessThanOp_Integer(Primitive *l, Primitive *r) {
    return primitiveGenerate_i(l->val_i < r->val_i);
}

Private Primitive * primitiveGreaterThanOp_Integer(Primitive *l, Primitive *r) {
    return primitiveGenerate_i(l->val_i > r->val_i);
}

Private Primitive * primitiveLessOrEqualOp_Integer(Primitive *l, Primitive *r) {
    return primitiveGenerate_i(l->val_i <= r->val_i);
}

Private Primitive * primitiveLessOrEqualOp_Integer(Primitive *l, Primitive *r) {
    return primitiveGenerate_i(l->val_i >= r->val_i);
}

Private Primitive * primitiveGreaterOrEqualOp_Integer(Primitive *l, Primitive *r) {
    return primitiveGenerate_i(l->val_i != r->val_i);
}
