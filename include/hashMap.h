/* hashMap.h */

#ifndef _HASH_MAP_H_
#define _HASH_MAP_H_

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
    _Bool     (*keyCmp)(void *key1, void *key2);
    _Status_t (*valRelease)(void *value);
    void *    (*valDup)(void *value);
} hashMapType;

typedef struct __hashMap {
    int size;
    int used;
    hashMapEntry **entries;
    hashMapEntry **last;
} __hashMap;

typedef enum {
    IN_RE_HASHING = 0,
    IN_NORMAL = 1
} rehashState;

typedef struct hashMap {
    // maps[0] is master map and maps[1] is slave
    // maps[1] is for rehashing purposes
    __hashMap maps[2];
    hashMapType *type;
    rehashState state;
    int rehashIdx;
} hashMap;

typedef struct entryPos {
    hashMapEntry **slot;
    hashMapEntry *entry;
} entryPos;

typedef struct hashMapIter {
    hashMap *map;
    entryPos pos;
    // 0: master, 1 : slave
    int table;
} hashMapIter;

/* Functions implement as macros */
#define hashMapGetSize(HM, idx) ((HM)->maps[idx].size)
#define hashMapGetEntry(HM, idx) ((HM)->maps[idx].entries)
#define hashMapHash(HM, key) ((HM)->type->hashing(key))

#define hashMapSizeMask(size) ((size) - 1)

#define hashMapIsInRehashing(HM) ((HM)->state == IN_RE_HASHING)
#define hashMapIsEmpty(HM) ((HM)->maps[0].used == 0)

#define hasEntryGetKey(HE) ((HE)->key)
#define hashEntrySetKey(HE, KEY) ((HE)->key = KEY)

#define hashEntryGetValue(HE) ((HE)->value)
#define hashEntrySetValue(HE, VALUE) ((HE)->value = VALUE)

#define hashEntryPosSlot(POS) ((POS)->slot)
#define hashEntryPosSetSlot(POS, S) ((POS)->slot = S)
#define hashEntryPosIsEmptySlot(S) (*(S) == null)

#define hashEntryPosEntry(POS) ((POS)->entry)
#define hashEntryPosSetEntry(POS, E) ((POS)->entry = E)

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

#ifdef _TEST_LAB_UNIT_TESTING_

void hashMap_Basic(void **state);

#endif /* _TEST_LAB_UNIT_TESTING */

#endif /* _HASH_MAP_H_ */
