/* hashMap.h */

#include <inttypes.h>
#include "type.h"

/* HashMap(m) ::=
 *   add: m * valueType(key) * valueType(val) -> _Status_t
 *   del: m * valueType(key) -> _Status_t
 *   Search: m * valueType(key) -> valueType(val)
 *   Dup: m -> m
 *   release: m -> _Status_t
 */

typedef struct hashMapEntry {
    void *key;
    void *value;
    struct hashMapEntry *next;
} hashMapEntry;

typedef struct hashMapType {
    uint64_t  (*hashing)(const void *key);
    _Status_t (*keyRelease)(void *key);
    void *    (*keyDup)(void *key);
    _Bool     (*keyCmp)(void *key);
    _Status_t (*valRelease)(void *value);
    void *    (*valDUp)(void *value);
} hashMapType;

typedef struct __hashMap {
    hashMapEntry **entries;
} __hashMap;

typedef enum {
    IN_RE_HASHING,
    IN_NORMAL
} rehashState;

typedef struct hashMap {
    int size;
    int used;
    // maps[0] is master map and maps[1] is slave
    // maps[1] is for rehashing purposes
    __hashMap *maps[2];
    hashMapType *type;
    rehashState state;
} hashMap;

typedef struct hashMapIter {
    hashMap *map;
    hashMapEntry *entry;
} hashMapIter;

/* Functions implement as macros */
#define hashMapGetSize(HM) ((HM)->size)
#define hashMapGetEntry(HM) ((HM)->entries)

#define hashMapIsInRehashing(HM) ((HM)->state == IN_RE_HASHING)

#define hasEntryGetKey(HE) ((HE)->key)
#define hashEntrySetKey(HE, KEY) ((HE)->key = KEY)

#define hashEntryGetValue(HE) ((HE)->value)
#define hashEntrySetValue(HE, VALUE) ((HE)->value = VALUE)

/* Prototype */
hashMap * hashMapCreate(hashMapType *type);
_Status_t hashMapRelease(hashMap *map);
_Status_t hashMapAdd(hashMap *map, const void *key, const void *val);
_Status_t hashMapDel(hashMap *map, const void *key);
void *    hashMapSearch(hashMap *map, const void *key);
hashMap * hashMapDup(hashMap *map);
hashMapEntry * hashMapNext(hashMapIter *iter);
hashMapEntry * hashMapPrev(hashMapIter *iter);

hashMapIter hashMapGetIter(hashMap *map);
hashMapIter hashMap_I_Successor(hashMapIter iter);
hashMapIter hashMap_I_Predecessor(hashMapIter iter);
