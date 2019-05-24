/* hashMap.c */

#include <malloc.h>

#include "type.h"
#include "hashMap.h"
#include "wrapper.h"

/* Private prototypes */
typedef _Bool (*keyCmp)(const void *key);
private hashMapEntry * __hashMapEntryPrev(hashMapEntry *entry);
private hashMapEntry * __hashMapEntryNext(hashMapEntry *entry);
private hashMapEntry * __hashMapEntryTail(hashMapEntry *entry);
private hashMapEntry * __hashMapEntrySearch(hashMapEntry *entry, const void *key, keyCmp cmp);
private _Status_t      __hashMapEntryInsert(hashMapEntry *entry, hashMapEntry *newEntry, int isAfter);
private _Status_t      __hashMapEntryAppend(hashMapEntry *entry, hashMapEntry *newEntry);

hashMap * hashMapCreate(hashMapType *type) {
    hashMap *map = (hashMap *)zMalloc(sizeof(hashMap));

    map->type = type;
    map->size = 0;
    map->used = 0;
    map->state = IN_NORMAL;
    map->maps[0] = null;
    map->maps[1] = null;

    return map;
}

_Status_t __hashMapRelease_helper(hashMapType *type, __hashMap *map) {
    hashMapEntry **entries, *currentEntry, *nextEntry;

    for (entries = map->entries; isNonNull(entries); ++entries) {
        nextEntry = *entries;
        while (currentEntry = nextEntry) {
            type->keyRelease(currentEntry->key);
            type->valRelease(currentEntry->value);

            nextEntry = __hashMapEntryNext(currentEntry);

            free(currentEntry);
        }
    }
}
_Status_t hashMapRelease(hashMap *map) {
    // 0 of elements is reside in hash map then just return
    if (map->used == 0) {
        return OK;
    }

    if (hashMapIsInRehashing(map)) {
        __hashMapRelease_helper(map->type, map->maps[1]);
    }

    __hashMapRelease_helper(map->type, map->maps[0]);

    return OK;
}

_Status_t hashMapAdd(hashMap *map, const void *key, const void *val) {

    return OK;
}

_Status_t hashMapDel(hashMap *map, const void *key) {

}

void * hashMapSearch(hashMap *map, const void *key) {

}

hashMap * hashMapDup(hashMap *map) {

}

hashMapEntry * hashMapNext(hashMapIter *iter) {

}

hashMapEntry * hashMapPrev(hashMapIter *iter) {

}

hashMapIter hashMapGetIter(hashMap *map) {

}

hashMapIter hashMap_I_Successor(hashMapIter iter) {

}

hashMapIter hashMap_I_Predecessor(hashMapIter iter) {

}

/* Private procedures */
private hashMapEntry * __hashMapEntryPrev(hashMapEntry *entry) {

}

private hashMapEntry * __hashMapEntryNext(hashMapEntry *entry) {

}

private hashMapEntry * __hashMapEntryTail(hashMapEntry *entry) {

}

private hashMapEntry * __hashMapEntrySearch(hashMapEntry *entry, const void *key, keyCmp cmp) {

}

private _Status_t __hashMapEntryInsert(hashMapEntry *entry, hashMapEntry *newEntry, int isAfter) {

}

private _Status_t __hashMapEntryAppend(hashMapEntry *entry, hashMapEntry *newEntry) {

}
