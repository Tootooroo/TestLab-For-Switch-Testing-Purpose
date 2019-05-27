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
    _Bool     (*keyCmp)(void *key, void *key);
    _Status_t (*valRelease)(void *value);
    void *    (*valDup)(void *value);
} hashMapType;

typedef struct __hashMap {
    int size;
    int used;
    hashMapEntry **entries;
} __hashMap;

typedef enum {
    IN_RE_HASHING,
    IN_NORMAL
} rehashState;

typedef struct hashMap {
    // maps[0] is master map and maps[1] is slave
    // maps[1] is for rehashing purposes
    __hashMap maps[2];
    hashMapType *type;
    rehashState state;
} hashMap;

typedef struct hashMapIter {
    hashMap *map;
    hashMapEntry *entry;
} hashMapIter;

/* Functions implement as macros */
#define hashMapGetSize(HM, idx) ((HM)->maps[idx].size)
#define hashMapGetEntry(HM, idx) ((HM)->maps[idx].entries)
#define hashMapHash(HM, key) ((HM)->type->hashing(key))

#define hashMapIsInRehashing(HM) ((HM)->state == IN_RE_HASHING)
#define hashMapIsEmpty(HM) ((HM)->maps[0].used == 0)

#define hasEntryGetKey(HE) ((HE)->key)
#define hashEntrySetKey(HE, KEY) ((HE)->key = KEY)

#define hashEntryGetValue(HE) ((HE)->value)
#define hashEntrySetValue(HE, VALUE) ((HE)->value = VALUE)

/* Prototype */
hashMap * hashMapCreate(hashMapType *type);
_Status_t hashMapRelease(hashMap *map);
_Status_t hashMapAdd(hashMap *map, void *key, void *val);
_Status_t hashMapDel(hashMap *map, void *key);
void *    hashMapSearch(hashMap *map, void *key);
hashMap * hashMapDup(hashMap *map);
hashMapEntry * hashMapNext(hashMapIter *iter);
hashMapEntry * hashMapPrev(hashMapIter *iter);

hashMapIter hashMapGetIter(hashMap *map);
hashMapIter hashMap_I_Successor(hashMapIter iter);
hashMapIter hashMap_I_Predecessor(hashMapIter iter);
