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

private _Bool scopeIsPrimitiveType(char *type);
private _Bool scopeIsObjectType(char *type, Scope *s);


/* Private Variable */
private hashMapType scopeTypeVar = {
    scopeHashing,
    scopeKeyRelease,
    scopeKeyDup,
    scopeKeyCmp,
    scopeValRelease,
    scopeValDup
};

private hashMapType scopeTypeFun = {
    scopeHashing,
    scopeKeyRelease,
    scopeKeyDup,
    scopeKeyCmp,
    NULL,
    NULL
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
    scopeReleaseCurrent(s);

    if (s->outer) scopeRelease(s->outer);

    free(s);
}

void scopeReleaseCurrent(Scope *s) {
    if (s->template) hashMapRelease(s->template);
    if (s->functions)    hashMapRelease(s->functions);
    if (s->primitives) hashMapRelease(s->primitives);
    if (s->objects)  hashMapRelease(s->objects);

}

_Status_t scopeNewFunc(Scope *s, pair *c) {
    if (!s->functions) s->functions = hashMapCreate(&scopeTypeVar);
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

_Status_t scopeNewTemplate(Scope *s, pair *p) {
    if (!s->template) s->template = hashMapCreate(&scopeTypeVar);
    return hashMapAdd(s->template, PAIR_GET_LEFT(p), PAIR_GET_RIGHT(p));
}

Func * scopeGetFunc(Scope *s, char *ident) {
    Func *f = NULL;

    if (!s->functions) goto UPPER_SCOPE;

    f = hashMapSearch(s->functions, (void *)ident);

UPPER_SCOPE:
    while (!f) {
        /* Recursive to search upper scope */
        s = s->outer;
        if (isNull(s)) break;

        f = hashMapSearch(s->functions, (void *)ident);
    }

    return f;
}

Variable * scopeGetPrimitive(Scope *s, char *ident) {
    Variable *v = NULL;

    if (!s->primitives) goto UPPER_SCOPE;
    v = hashMapSearch(s->primitives, (void *)ident);

UPPER_SCOPE:
    while (!v) {
        /* Recursive search to upper scope */
        s = s->outer;
        if (isNull(s)) break;

        v = hashMapSearch(s->primitives, (void *)ident);
    }

    return v;
}

Variable * scopeGetObject(Scope *s, char *ident) {
    Variable *v = NULL;

    if (!s->objects) goto UPPER_SCOPE;
    v = hashMapSearch(s->objects, (void *)ident);

UPPER_SCOPE:
    while (!v) {
        /* Recursive search to upper scope */
        s = s->outer;
        if (isNull(s)) break;

        v = hashMapSearch(s->objects, (void *)ident);
    }

    return v;
}

Template * scopeGetTemplate(Scope *s, char *ident) {
    Template *t = NULL;

    if (!s->template) goto UPPER;
    t = hashMapSearch(s->template, (void *)ident);
UPPER:
    while (!t) {
        /* Recursive search to upper scope */
        s = s->outer;
        if (isNull(s)) break;

        t = hashMapSearch(s->template, (void *)ident);
    }

    return t;
}

_Bool scopeIsValidType(char *type, Scope *s) {
    return scopeIsPrimitiveType(type) || scopeIsObjectType(type, s);
}

/* Private Procedures */
private _Bool scopeIsPrimitiveType(char *type) {
    _Bool isPrimitive =
        strCompare(type, "Int") ||
        strCompare(type, "String") ||
        strCompare(type, "Ops");
    return isPrimitive;
}

private _Bool scopeIsObjectType(char *type, Scope *s) {
    /* Search the type in template of scope */
    void *e = NULL;

    if (!s->template) goto UPPER;
    e = hashMapSearch(s->template, (void *)type);

 UPPER:
    while (!e) {
        s = s->outer;
        if (!s) break;

        e = hashMapSearch(s->template, (void *)type);
    }
    return e;
}

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

private _Status_t scopeFunRelease(void *value) {
    return OK;
}

private void * scopeFunDup(void *value) {
    return null;
}

#ifdef _AST_TREE_TESTING_

#include "test.h"

void scopeTest(void **state) {

}

#endif /* _AST_TREE_TESTING_ */
