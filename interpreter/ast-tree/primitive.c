/* primitive.c */

#include "primitive.h"
#include "wrapper.h"
#include "string.h"

/* Private prototypes */
private Primitive * primitiveGenerate_i(int i);
private Primitive * primitiveGenerate_s(char *s);
private Primitive * primitiveGenerate_op(Ops *op);

/* Public procedures */
Primitive * PrimitiveGen(void *val, primitiveType type) {
    switch (type) {
    case PRIMITIVE_TYPE_INT :
        return primitiveGenerate_i((int64_t)val);
        break;
    case PRIMITIVE_TYPE_STR :
        return primitiveGenerate_s((char *)val);
        break;
    case PRIMITIVE_TYPE_OPS :
        return primitiveGenerate_op((Ops *)val);
        break;
    }

    return NULL;
}

Primitive * primitiveDup(Primitive *orig) {
    primitiveType type;
    Primitive *dup = (Primitive *)zMalloc(sizeof(Primitive));

    type = dup->type = orig->type;
    if (type == PRIMITIVE_TYPE_INT) {
        dup->val_i = orig->val_i;
    } else if (type == PRIMITIVE_TYPE_STR) {
        dup->val_str = strdup(orig->val_str);
    } else {
        dup->ops->ops = listDup(orig->ops->ops);
    }

    return dup;
}

_Status_t primitiveRelease(Primitive *p) {
    if (isPrimitive_str(p))
        free(p->val_str);
    else if (isPrimitive_ops(p))
        listRelease(p->ops->ops);

    free(p);

    return OK;
}

// Ops procedures
Ops * opsDefault() {
    return (Ops *)zMalloc(sizeof(Ops));
}

_Bool opsIsSupportOp(Ops *ops, char *opName) {
    return listSearch(ops->ops, (void *)opName) != NULL;
}

/* Private procedures */
private Primitive * primitiveGenerate_i(int i) {
    Primitive *p = (Primitive *)zMalloc(sizeof(Primitive));

    p->type = PRIMITIVE_TYPE_INT;
    p->val_i = i;

    return p;
}

private Primitive * primitiveGenerate_s(char *s) {
    Primitive *p = (Primitive *)zMalloc(sizeof(Primitive));

    p->type = PRIMITIVE_TYPE_STR;
    p->val_str = s;

    return p;
}

private Primitive * primitiveGenerate_op(Ops *op) {
    Primitive *p = (Primitive *)zMalloc(sizeof(Primitive));

    p->type = PRIMITIVE_TYPE_OPS;
    p->ops = op;

    return p;
}
