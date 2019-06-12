/* scope.c */

#include "type.h"
#include "scope.h"
#include "wrapper.h"
#include "string.h"

/* Private Prototypes */
private uint64_t scopeHashing(const void *key);
private _Status_t scopeKeyRelease(void *key);
private void * scopeKeyDup(void *key);
private _Bool scopeKeyCmp(void *keyl, void *keyr);

private _Status_t scopeValRelease(void *value);
private void * scopeValDup(void *value);

/* Private Variable */
hashMapType scopeTypePrimitive = {
    scopeHashing,
    scopeKeyRelease,
    scopeKeyDup,
    scopeKeyCmp,
    scopeValRelease,
    scopeValDup
};

/* Public Procedures */
scope * scopeGenerate() {
    scope *s = (scope *)zMalloc(sizeof(scope));

    return s;
}

scope * subScopeGenerate(scope *s) {
    scope *sub = (scope *)zMalloc(sizeof(scope));
    sub->outer = s;

    return sub;
}

_Status_t scopeNewCase(scope *s, Case *c) {

}

_Status_t scopeNewPrimitive(scope *s, Primitive *p) {

}

_Status_t scopeNewObject(scope *s, Object *o) {

}

/* Private Procedures */
private uint64_t scopeHashing(const void *key) {
    uint64_t hashVal = 0, val;
    char *key_str = (char *)key;
    char *current = key_str;

    while ((val = *current) != '\n') {
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

    int keyl_len = strlen(keyl_str),
        keyr_len = strlen(keyr_str);

    if (keyl_len != keyr_len) return false;

    return !strncmp(keyl_str, keyr_str, keyl_len);
}

private _Status_t scopeValRelease(void *value) { return OK; }

private void * scopeValDup(void *value) { return null; }
