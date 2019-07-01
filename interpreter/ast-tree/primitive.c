/* primitive.c */

#include "primitive.h"
#include "wrapper.h"


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
