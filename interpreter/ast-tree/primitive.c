/* primitive.c */

#include "primitive.h"
#include "wrapper.h"

/* Private prototypes */
Private Primitive * primitivePlusOp_Integer(Primitive *l, Primitive *r);
Private Primitive * primitiveMinusOp_Integer(Primitive *l, Primitive *r);


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

/* Private procedures */
Private Primitive * primitivePlusOp_Integer(Primitive *l, Primitive *r) {
    int sum = l->val_i + r->val_i;
    return primitiveGenerate_i(sum);
}

Private Primitive * primitiveMinusOp_Integer(Primitive *l, Primitive *r) {
    int diff = l->val_i - r->val_i;
    return primitiveGenerate_i(diff);
}
