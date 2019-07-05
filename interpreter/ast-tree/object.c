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
    o->members = hashMapCreate(&obj);

    return o;
}

Object * objDup(Object *orig) {
    Object *dup = objDefault();

    OBJ_SET_IDENTIFIER(dup, OBJ_IDENTIFIER(orig));
    OBJ_SET_TYPE(dup, OBJ_TYPE(orig));
    OBJ_SET_MEMBERS(dup, hashMapDup(OBJ_MEMBERS(orig)));

    return dup;
}

Variable * objGetMember(Object *o, char *member) {
    return hashMapSearch(o->members, (void *)member);
}

/* fixme: Need to implement objectRelease() */
void objectRelease(Object *o) {
    if (o->objectType) free(o->objectType);
    if (o->identifier) free(o->identifier);
    if (o->members)    hashMapRelease(o->members);
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
    return strCompare((char *)keyl, (char *)keyr);
}

private _Status_t objValRelease(void *val) {
    varRelease((Variable *)val);

    return OK;
}

/* fixme: Before that you should implement duplication of
 * variable. */
private void * objValDup(void *val) {
    return varDup(val);
}
