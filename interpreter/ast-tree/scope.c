/* scope.c */

#include "type.h"
#include "scope.h"
#include "wrapper.h"
#include "string.h"
#include "pair.h"

/* Private Prototypes */
private uint64_t scopeHashing(const void *key);
private _Status_t scopeKeyRelease(void *key);
private void * scopeKeyDup(void *key);
private _Bool scopeKeyCmp(void *keyl, void *keyr);

private _Status_t scopeValRelease(void *value);
private void * scopeValDup(void *value);

/* Private Variable */
private hashMapType scopeTypeVar = {
    scopeHashing,
    scopeKeyRelease,
    scopeKeyDup,
    scopeKeyCmp,
    scopeValRelease,
    scopeValDup
};

/* Public Procedures */
Scope * scopeGenerate() {
    Scope *s = (Scope *)zMalloc(sizeof(Scope));

    return s;
}

Scope * subScopeGenerate(Scope *s) {
    Scope *sub = (Scope *)zMalloc(sizeof(Scope));
    sub->outer = s;

    return sub;
}

void scopeRelease(Scope *s) {
    if (s->template) hashMapRelease(s->template);
    if (s->functions)    hashMapRelease(s->functions);
    if (s->primitives) hashMapRelease(s->primitives);
    if (s->objects)  hashMapRelease(s->objects);

    if (s->outer) scopeRelease(s->outer);
}

_Status_t scopeNewCase(Scope *s, pair *c) {
    return hashMapAdd(s->functions, PAIR_GET_LEFT(c), PAIR_GET_RIGHT(c));
}

/* Type of pair is (Identifier, Primitive) */
_Status_t scopeNewPrimitive(Scope *s, pair *p) {
    if (!s->primitives) s->primitives = hashMapCreate(&scopeTypeVar);
    // To check that is the identifier be used by an object.
    if (s->objects && hashMapSearch(s->objects, PAIR_GET_LEFT(p))) return ERROR;

    return hashMapAdd(s->primitives, PAIR_GET_LEFT(p), PAIR_GET_RIGHT(p));
}

_Status_t scopeNewObject(Scope *s, pair *p) {
    if (!s->objects) s->objects = hashMapCreate(&scopeTypeVar);
    // To check that is the identifier be used by an primitive.
    if (s->primitives && hashMapSearch(s->primitives, PAIR_GET_LEFT(p))) return ERROR;

    return hashMapAdd(s->objects, PAIR_GET_LEFT(p), PAIR_GET_RIGHT(p));
}

/* Private Procedures */
private uint64_t scopeHashing(const void *key) {
    uint64_t hashVal = 0, val;
    char *key_str = (char *)key;
    char *current = key_str;

    while ((val = *current) != '\0') {
        hashVal += val;
        ++current;
    }

    return hashVal;
}

private _Status_t scopeKeyRelease(void *key) {
    free(key);
    return OK;
}

private void *scopeKeyDup(void *key) {
    char *key_str = key;

    return (void *)strdup(key_str);
}

private _Bool scopeKeyCmp(void *keyl, void *keyr) {
    char *keyl_str = keyl, *keyr_str = keyr;

    return strCompare(keyl_str, keyr_str);

}

private _Status_t scopeValRelease(void *value) {

    return OK;
}

private void * scopeValDup(void *value) {
    return null;
}

#ifdef _AST_TREE_TESTING_

#include "test.h"

void scopeTest(void **state) {

}

#endif /* _AST_TREE_TESTING_ */
