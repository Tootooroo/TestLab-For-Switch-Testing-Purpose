/* scope.c */

#include "type.h"
#include "scope.h"
#include "wrapper.h"
#include "string.h"
#include "pair.h"
#include "module.h"

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
    if (s->template)   hashMapRelease(s->template);
    if (s->functions)  hashMapRelease(s->functions);
    if (s->primitives) hashMapRelease(s->primitives);
    if (s->objects)    hashMapRelease(s->objects);

}

hashMap * scopeGetEntities(Scope *s, int type) {
    switch (type) {
    case TEMPLATE_ENTITY:
        return s->template;
        break;
    case FUNCTION_ENTITY:
        return s->functions;
        break;
    case PRIMITIVE_ENTITY:
        return s->primitives;
        break;
    case OBJECT_ENTITY:
        return s->objects;
        break;
    }

    return NULL;
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

private void * scopeGet_Recursive(Scope *s, char *ident, int type) {
    void *v = NULL;
    hashMap *map = scopeGetEntities(s, type);

    if (map == NULL) goto RECURSIVE_SEARCH;
    v = hashMapSearch(map, (void *)ident);

 RECURSIVE_SEARCH:
    while (!v) {
        s = s->outer;
        if (isNull(s)) break;

        if ((map = scopeGetEntities(s, type)) != NULL)
            v = hashMapSearch(map, (void *)ident);
    }

    return v;
}

Func * scopeGetFunc(Scope *s, char *ident) {
    Func *f = scopeGet_Recursive(s, ident, FUNCTION_ENTITY);

    if (!f) f = modTblSearchFunction(&moduleTable, NULL, ident);

    return f;
}

Variable * scopeGetPrimitive(Scope *s, char *ident) {
    Variable *v = scopeGet_Recursive(s, ident, PRIMITIVE_ENTITY);

    if (!v) v = modTblSearchPrimitive(&moduleTable, NULL, ident);

    return v;
}

Variable * scopeGetObject(Scope *s, char *ident) {
    Variable *v = scopeGet_Recursive(s, ident, OBJECT_ENTITY);

    if (!v) v = modTblSearchObject(&moduleTable, NULL, ident);

    return v;
}

Template * scopeGetTemplate(Scope *s, char *ident) {
    Template *t = scopeGet_Recursive(s, ident, TEMPLATE_ENTITY);

    if (!t) t = modTblSearchTemplate(&moduleTable, NULL, ident);

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
