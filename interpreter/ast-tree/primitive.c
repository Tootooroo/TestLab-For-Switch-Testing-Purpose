/* primitive.c */

#include "primitive.h"
#include "wrapper.h"
#include "string.h"

/* Private prototypes */
private Primitive * primitiveGenerate_i(int i);
private Primitive * primitiveGenerate_s(char *s);
private Primitive * primitiveGenerate_op(Ops *op);

private _Status_t opsListRelease(void *);
private _Bool     opsListMatch(const void *, const void *);
private void *    opsListDup(void *);

/* Public procedures */
Primitive * primitiveDefault(primitiveType type) {
    Primitive *p = (Primitive *)zMalloc(sizeof(Primitive));
    return p;
}

Primitive * primitiveGen(void *val, primitiveType type) {
    switch (type) {
    case PRIMITIVE_TYPE_INT :
        return primitiveGenerate_i(*(int *)val);
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
        dup->ops = opsDup(orig->ops);
    }

    return dup;
}

_Status_t primitiveRelease(Primitive *p) {
    if (isPrimitive_str(p))
        free(p->val_str);
    else if (isPrimitive_ops(p))
        opsRelease(p->ops);

    free(p);

    return OK;
}

// Assign value of r to l.
_Status_t primitiveAssign(Primitive *l, Primitive *r) {
    primitiveType type = r->type;

    if (l->type == PRIMITIVE_TYPE_STR) {
        free(l->val_str);
    } else if (l->type == PRIMITIVE_TYPE_OPS) {
        opsRelease(l->ops);
    }

    l->type = r->type;

    switch(type) {
    case PRIMITIVE_TYPE_INT:
        l->val_i = r->val_i;
        break;
    case PRIMITIVE_TYPE_STR:
        if (l->val_str) free(l->val_str);

        l->val_str = strdup(r->val_str);
        break;
    case PRIMITIVE_TYPE_OPS:
        if (l->ops) opsRelease(l->ops);
        l->ops = opsDup(r->ops);
        break;
    default:
        return ERROR;
    }

    return OK;
}

_Bool primitiveIsEqual(Primitive *l, Primitive *r) {
    primitiveType type = l->type;

    if (type != r->type) return false;

    switch (type) {
    case PRIMITIVE_TYPE_INT:
        return l->val_i == r->val_i;
        break;
    case PRIMITIVE_TYPE_STR:
        return strCompare(l->val_str, r->val_str);
        break;
    case PRIMITIVE_TYPE_OPS:
        return opsIsEqual(l->ops, r->ops);
    default:
        return false;
    }

    return true;
}

// Ops procedures
Ops * opsDefault() {
    Ops *p = (Ops *)zMalloc(sizeof(Ops));
    return p;
}

_Status_t opsNewMember(Ops *op, char *member) {

    // Initialize list.
    if (!op->ops) {
        list *l = listCreate();

        listSetReleaseMethod(l, opsListRelease);
        listSetMatchMethod(l, opsListMatch);
        listSetDupMethod(l, opsListDup);

        op->ops = l;
    }

    listAppend(op->ops, member);

    return OK;
}

_Status_t opsRmMember(Ops *op, char *member) {
    if (!op->ops) return ERROR;

    return listDelNode(op->ops, member);
}

void opsRelease(Ops *op) {
    if (op->ops) listRelease(op->ops);
}

_Bool opsIsSupportOp(Ops *ops, char *opName) {
    if (ops->ops)
        return listSearch(ops->ops, (void *)opName) != NULL;

    return false;
}

Ops *opsDup(Ops *orig) {
    Ops *dup = opsDefault();

    dup->ops = listDup(orig->ops);

    return dup;
}

_Status_t opsAssign(Ops *l, Ops *r) {
    // Release the exists list
    if (l->ops) listRelease(l->ops);

    l->ops = listDup(r->ops);

    return OK;
}

_Bool opsIsEqual(Ops *l, Ops *r) {
    if (isNull(l->ops) && isNull(r->ops))
        return true;
    if (isNull(l->ops) || isNull(r->ops))
        return false;

    return listIsEqual(l->ops, r->ops);
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

private _Status_t opsListRelease(void *c) {
    free(c);
    return OK;
}

private _Bool opsListMatch(const void *l, const void *r) {
    return strCompare((char *)l, (char *)r);
}

private void * opsListDup(void *orig) {
    return strdup((char *)orig);
}

#ifdef _AST_TREE_TESTING_

#include "test.h"

void primitiveTest(void **state) {
    primitiveInteger_Test();
    primitiveString_Test();
    primitiveOps_Test();
}

void primitiveInteger_Test(void) {
    int num1 = 1;

    Primitive *p1 = primitiveGen(&num1, PRIMITIVE_TYPE_INT);

    /* Duplication */
    Primitive *p2 = primitiveDup(p1);

    /* Equality */
    assert_int_equal(primitiveIsEqual(p1, p2), true);

    /* Assign */
    Primitive *p3 = primitiveDefault();
    primitiveAssign(p3, p1);
    assert_int_equal(primitiveIsEqual(p1, p3), true);
}

void primitiveString_Test(void) {
    char *str1 = "Hellow";

    Primitive *p1 = primitiveGen(str1, PRIMITIVE_TYPE_STR);

    /* Duplication */
    Primitive *p2 = primitiveDup(p1);

    /* Equality */
    assert_int_equal(primitiveIsEqual(p1, p2), true);

    /* Assign */
    Primitive *p3 = primitiveDefault();
    primitiveAssign(p3, p1);
    assert_int_equal(primitiveIsEqual(p1, p3), true);
}

void primitiveOps_Test(void) {
    Ops *op = opsDefault();

    opsNewMember(op, "Hello");
    opsNewMember(op, "Hey");

    Primitive *p1 = primitiveGen(op, PRIMITIVE_TYPE_OPS);

    /* Duplication */
    Primitive *p2 = primitiveDup(p1);

    /* Equality */
    assert_int_equal(primitiveIsEqual(p1, p2), true);

    /* Asign */
    Primitive *p3 = primitiveDefault();
    primitiveAssign(p3, p1);
    assert_int_equal(primitiveIsEqual(p1, p3), true);
}

#endif /* _AST_TREE_TESTING_ */
