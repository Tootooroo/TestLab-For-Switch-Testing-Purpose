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
    p->val_i = s;

    return p;
}

Primitive * primitiveGenerate_op(Ops op) {
    Primitive *p = (Primitive *)zMalloc(sizeof(Primitive));

    p->type = PRIMITIVE_TYPE_OPS;
    p->ops = op;

    return p;
}
