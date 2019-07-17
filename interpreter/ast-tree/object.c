/* object.c */
#include "object.h"
#include "wrapper.h"
#include "hashMap.h"
#include "string.h"
#include "variable.h"

/* Private prototypes */
private uint64_t objHashing(const void *key);
private _Status_t objKeyRelease(void *key);
private void * objKeyDup(void *key);
private _Bool objKeyCmp(void *keyl, void *keyr);
private _Status_t objValRelease(void *val);
private void * objValDup(void *val);
private _Status_t __memberRelease(void *);
private _Bool __memberMatch(const void *, const void *);
private void * __memberDup(void *);

/* Private variables */
hashMapType obj = {
    .hashing = objHashing,
    .keyRelease = objKeyRelease,
    .keyDup = objKeyDup,
    .keyCmp = objKeyCmp,
    .valRelease = objValRelease,
    .valDup = objValDup
};


/* Public procedures */
Object * objDefault() {
    Object *o = (Object *)zMalloc(sizeof(Object));
    return o;
}

Object * objGen(char *identifier, char *type) {
    Object *o = objDefault();

    OBJ_SET_IDENTIFIER(o, identifier);
    OBJ_SET_TYPE(o, type);

    return o;
}

Object * objDup(Object *orig) {
    Object *dup = objDefault();

    OBJ_SET_IDENTIFIER(dup, OBJ_IDENTIFIER(orig));
    OBJ_SET_TYPE(dup, OBJ_TYPE(orig));
    OBJ_SET_MEMBERS(dup, listDup(orig->members));

    return dup;
}

Variable * objGetMember(Object *o, char *member) {
    listNode *node = listSearch(o->members, member);
    return (Variable *)node->value;
}

/* fixme: Need to implement objectRelease() */
void objectRelease(Object *o) {
    if (o->objectType) free(o->objectType);
    if (o->identifier) free(o->identifier);
    if (o->members)    listRelease(o->members);
}

// Template
Template * templateDefault() {
    return objDefault();
}

Template * templateGen(char *type) {
    return objGen(NULL, type);
}

Template * templateDup(Template *t) {
    return objDup(t);
}

Variable * templateGetMember(Template *t, char *ident) {
    return objGetMember(t, ident);
}

_Status_t templateAddMember(Template *t, Variable *var) {
    list *member = t->members;

    if (isNull(member)) {
        member = listCreate();

        listSetReleaseMethod(member, __memberRelease);
        listSetMatchMethod(member, __memberMatch);
        listSetDupMethod(member, __memberDup);

        t->members = member;
    }

    listAppend(member, var);

    return OK;
}

_Status_t templateAddMembers(Template *t, list *vars) {
    list *member = t->members;

    if (isNull(member)) {
        member = listCreate();

        listSetReleaseMethod(member, __memberRelease);
        listSetMatchMethod(member, __memberMatch);
        listSetDupMethod(member, __memberDup);

        t->members = member;
    }

    listJoin(member, vars);

    return OK;
}

void templateRelease(Template *t) {
    objectRelease(t);
}

// Misc
_Bool objTypeCmp(Object *o1, Object *o2) {
    return strCompare(o1->objectType, o2->objectType);
}

Object * template2Object(Template *t) {
    Object *orig = (Object *)t;

    return templateDup(orig);
}

/* Private procedures */
private uint64_t objHashing(const void *key) {
    uint64_t hashVal = 0, val;
    char *current = (char *)key;

    while ((val = *current) != '\0') {
        hashVal += (val << 5) + val;
    }

    return hashVal;
}

private _Status_t objKeyRelease(void *key) {
    free(key);
    return true;
}

private void *objKeyDup(void *key) {
    return strdup((char *)key);
}

private _Bool objKeyCmp(void *keyl, void *keyr) {
    Variable *var = (Variable *)keyl;
    char *str = (char *)keyr;

    return strCompare(var->identifier, str);
}

private _Status_t objValRelease(void *val) {
    varRelease((Variable *)val);

    return OK;
}

/* fixme: Before that you should implement duplication of
 * variable. */
private void * objValDup( void *val) {
    return varDup(val);
}

private _Status_t __memberRelease(void *m) {
    Variable *var = (Variable *)m;

    varRelease(var);
    return OK;
}

private _Bool __memberMatch(const void *m, const void *name) {
    Variable *var = (Variable *)m;

    return varIdentCmp(var, (char *)name);
}

private void * __memberDup(void *var) {
    return varDup(var);
}
