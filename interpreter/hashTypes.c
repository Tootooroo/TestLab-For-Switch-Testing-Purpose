/* hashTypes.c */

#include "type.h"
#include "hashTypes.h"

/* Private prototypes */
private uint64_t scopeHashing(const void *key);
private _Status_t scopeKeyRelease(void *key);
private void * scopeKeyDup(void *key);
private _Bool scopeKeyCmp(void *keyl, void *keyr);
private _Status_t scopeValRelease(void *value);
private void * scopeValDup(void *value);

/* Public variables */
hashMapType scopeTypeVar = {
    scopeHashing,
    scopeKeyRelease,
    scopeKeyDup,
    scopeValRelease,
    scopeValDup
};

hashMapType scopeTypeFun = {
    scopeHashing,
    scopeKeyRelease,
    scopeKeyDup,
    scopeKeyCmp,
    NULL,
    NULL
};

/* Private procedures */
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
