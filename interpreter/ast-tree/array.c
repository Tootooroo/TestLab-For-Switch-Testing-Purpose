/* array.c */

#include "array.h"
#include "wrapper.h"
#include "variable.h"
#include <string.h>

/* Private Prototypes */
private Variable * arrayPlus(Variable *, Variable *);
private Variable * arrayAssign(Variable *, Variable *);

/* Public Variables */
VarOps arrayOperations = {
    arrayPlus, // Plus
    NULL, // Minus
    NULL, // Multiplication
    NULL, // Division
    NULL, // Equal
    NULL, // LessThan
    NULL, // GreaterThan
    NULL, // LessOrEqual
    NULL, // GreaterOrEqual
    NULL, // NotEqual
    arrayAssign, // Assign
    NULL  // Dot
};

/* Private procedures */
#define ARRAY_ELEMS(A) ((A)->elems)

/* Public procedures */
Array * createArray(char *type) {
    Array *a = (Array *)zMalloc(sizeof(Array));

    ARRAY_SET_TYPE(a, type);
    return a;
}

Variable * arraySelectViaIdx(Array *a, int index) {
    if (index >= ARRAY_NUM_OF_ELEMS(a))
        return NULL;

    list *elems = ARRAY_ELEMS(a);
    listIter iter = listGetIter(elems, LITER_FORWARD);

    Variable *v;

    do { v = listNext_v(&iter); } while (index-- > 0);

    return v;
}

void arraySetViaIdex(Array *a, Variable *v, int index) {
    Variable *v_ = arraySelectViaIdx(a, index);

    if (v_) varAssign_(v_, v);
}

void arrayPush(Array *a, Variable *v) {
    if (ARRAY_ELEMS(a) == NULL)
        ARRAY_ELEMS(a) = listCreate();

    listPush(ARRAY_ELEMS(a), v);
    ARRAY_NUM_OF_ELEMS_PLUS(a);
}

Variable * arrayPop(Array *a) {
    if (ARRAY_NUM_OF_ELEMS(a) < 1) return NULL;

    Variable *v = (Variable *)listPop(ARRAY_ELEMS(a));

    if (v) { ARRAY_NUM_OF_ELEMS_DESC(a); return v; }

    return NULL;
}

void arrayAppend(Array *a, Variable *v) {
    if (ARRAY_ELEMS(a) == NULL)
        ARRAY_ELEMS(a) = listCreate();

    listAppend(ARRAY_ELEMS(a), v);
    ARRAY_NUM_OF_ELEMS_PLUS(a);
}

Variable * arrayRetrive(Array *a) {
    if (ARRAY_NUM_OF_ELEMS(a) < 1) return NULL;
    Variable *v = (Variable *)listRetrive(ARRAY_ELEMS(a));

    if (v) { ARRAY_NUM_OF_ELEMS_DESC(a); return v; }

    return NULL;
}

private Variable * arrayPlus(Variable *l, Variable *r) {
    if (VAR_TYPE(l) != VAR_TYPE(r))
        abortWithMsg("Array plus operation failed");

    char *type = strdup(ARRAY_TYPE(l->array));
    Array *newArray = createArray(type);

    list *l_n = listDup(l->array->elems);
    listIter iter = listGetIter(r->array->elems, LITER_FORWARD);

    Variable *v;
    while ((v = listNext_v(&iter)) != NULL) {
        listAppend(l_n, varDup(v));
    }

    newArray->elems = l_n;

    return varGen(NULL, VAR_ARRAY, newArray);
}

private Variable * arrayAssign(Variable *l, Variable *r) {
    varAssign_(l, r);
    return varDup(l);
}

void arrayRelease(Array *a) {
    if (a->type) free(a->type);
    listRelease(a->elems);
}

#ifdef _AST_TREE_TESTING_

#include "test.h"

void arrayTesting(void **state) {
    Array *a = createArray("Int");

    int a_int = 1;

    // arrayPush test
    arrayPush(a, varGen(NULL, VAR_PRIMITIVE_INT, &a_int));

    Variable *v = arrayPop(a);
    assert_int_equal(a_int, VAR_GET_PRIMITIVE_INT(v));

    assert_non_null(!arrayPop(a));

    // arrayAppend test
    arrayAppend(a, varGen(NULL, VAR_PRIMITIVE_INT, &a_int));
    v = arrayRetrive(a);

    assert_int_equal(a_int, VAR_GET_PRIMITIVE_INT(v));

    int a_int_2 = 2;
    arrayAppend(a, varGen(NULL, VAR_PRIMITIVE_INT, &a_int));
    arrayAppend(a, varGen(NULL, VAR_PRIMITIVE_INT, &a_int));

    int a_int_1_e = 4, a_int_2_e = 5;
    arraySetViaIdex(a, varGen(NULL, VAR_PRIMITIVE_INT, &a_int_1_e), 0);
    arraySetViaIdex(a, varGen(NULL, VAR_PRIMITIVE_INT, &a_int_2_e), 1);
    v = arraySelectViaIdx(a, 0);
    assert_int_equal(a_int_1_e, VAR_GET_PRIMITIVE_INT(v));

    v = arraySelectViaIdx(a, 1);
    assert_int_equal(a_int_2_e, VAR_GET_PRIMITIVE_INT(v));
}

#endif /* _AST_TREE_TESTING_ */
