/* hashMap.c */

#include <malloc.h>
#include <string.h>

#include "type.h"
#include "hashMap.h"
#include "wrapper.h"

/* Private prototypes */
typedef _Bool (*keyCmp)(const void *key);
private hashMapEntry * __hashMapEntryPrev(hashMapEntry *entry);
private hashMapEntry * __hashMapEntryNext(hashMapEntry *entry);
private hashMapEntry * __hashMapEntryTail(hashMapEntry *entry);
private hashMapEntry * __hashMapEntrySearch(hashMapEntry *entry, void *key, keyCmp cmp);
private _Status_t      __hashMapEntryInsert(hashMapEntry *entry, hashMapEntry *newEntry, int isAfter);
private _Status_t      __hashMapEntryAppend(hashMapEntry *entry, hashMapEntry *newEntry);
private _Status_t      __hashMapEntryDel(hashMapEntry *entry);
private hashMapEntry * __hashMapEntryDup(hashMapEntry *entry, void *k_dup, void *v_dup);

private _Status_t __hashMapRelease_do(hashMapType *tpe, __hashMap *map);
private _Bool __hashMapIsNeedExpand(hashMap *map);
private _Status_t __hashMapExpand(hashMap *map);
private _Status_t __hashMapRehashingStep(hashMap *map);
private _Status_t __hashMapRehashing(hashMap *map);
private _Status_t __hashMapAdd(__hashMap *map, void *key, void *val);
private _Status_t __hashMapDel(__hashMap *map, void *key, hashMap *env);

hashMap * hashMapCreate(hashMapType *type) {
    hashMap *map = (hashMap *)zMalloc(sizeof(hashMap));

    map->type = type;
    map->state = IN_NORMAL;
    memset(map->maps, 0, sizeof(__hashMap) * 2);

    return map;
}

_Status_t hashMapRelease(hashMap *map) {
    // 0 of elements is reside in hash map then just return
    if (map->maps[0].used == 0) {
        return OK;
    }

    if (hashMapIsInRehashing(map)) {
        __hashMapRelease_do(map->type, &map->maps[1]);
    }

    return __hashMapRelease_do(map->type, &map->maps[0]);
}

_Status_t hashMapAdd(hashMap *map, void *key, void *val) {
    __hashMap *currentMap;

    if (__hashMapIsNeedExpand(map)) __hashMapExpand(map);
    if (hashMapIsInRehashing(map)) __hashMapRehashingStep(map);

    if (hashMapIsInRehashing(map))
        currentMap = &map->maps[1];
    else
        currentMap = &map->maps[0];

    uint64_t hashIdx = hashMapHash(map, key) & currentMap->size;
    hashMapEntry *newEntry = zMalloc(sizeof(hashMapEntry));
    hashEntrySetKey(newEntry, key);
    hashEntrySetValue(newEntry, val);

    newEntry->next = currentMap->entries[hashIdx];
    currentMap->entries[hashIdx] = newEntry;

    return OK;
}

_Status_t hashMapDel(hashMap *map, void *key) {
    if (hashMapIsEmpty(map)) return OK;
    if (hashMapIsInRehashing(map)) __hashMapRehashingStep(map);

    if (__hashMapDel(&map->maps[0], key, map) == OK)
        return OK;
    if (hashMapIsInRehashing(map)) {
        return __hashMapDel(&map->maps[1], key, map);
    } else {
        return ERROR;
    }
}

// return value in the hashMapEntry
void * hashMapSearch(hashMap *map, void *key) {
    if (hashMapIsEmpty(map)) return null;
    if (hashMapIsInRehashing(map)) __hashMapRehashingStep(map);

    uint64_t index_ = hashMapHash(map, key), index;

    hashMapEntry *entry;
    __hashMap *subMap = map->maps;
    __hashMap *currentMap;
    for (int i = 0; i < 2; i++) {
        currentMap = subMap + i;

        index = index_ & currentMap->size;
        entry = currentMap->entries[index];

        if (entry == null)
            break;

        do {
            if (map->type->keyCmp(key, entry->key))
                return entry;
        } while (entry = __hashMapEntryNext(entry));

        if (!hashMapIsInRehashing(map)) break;
    }

    return null;
}

hashMap * hashMapDup(hashMap *map) {
    hashMap *map_dup = hashMapCreate(map->type);

    int size, idx;
    __hashMap *subMap = map->maps;

    for (int i = 0; i < 2; i++) {
        map_dup->maps[i].size = subMap[i].size;
        map_dup->maps[i].used = subMap[i].used;

        if (subMap[i].size == 0 || subMap[i].used == 0)
            break;

        map_dup->maps[i].entries = zMalloc(sizeof(hashMapEntry *) * subMap[i].size);

        idx = 0;
        size = subMap[i].size;
        while (size--) {
            if (subMap->entries[idx] == null)
                continue;

            map_dup->maps[i].entries[idx] = __hashMapEntryDup(subMap[i].entries[idx],
                                                              map->type->keyDup, map->type->valDup);
            idx++;
        }
        if (!hashMapIsInRehashing(map)) break;
    }
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

// HashMap procedures
private _Status_t __hashMapRelease_do(hashMapType *type, __hashMap *map) {
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
    return OK;
}

private _Status_t __hashMapExpand(hashMap *map) {
    return OK;
}

private _Bool __hashMapIsNeedExpand(hashMap *map) {
    return true;
}

private _Status_t __hashMapRehashingStep(hashMap *map) {
    return OK;
}

private _Status_t __hashMapRehashing(hashMap *map) {
    return OK;
}

private _Status_t __hashMapAdd(__hashMap *map, void *key, void *val) {
    return OK;
}

private _Status_t __hashMapDel(__hashMap *map, void *key, hashMap *env) {
    uint64_t index = hashMapHash(env, key) & map->size;
    hashMapEntry *entryHead, *entryBeDel;

    entryHead = map->entries[index];
    entryBeDel = __hashMapEntrySearch(entryHead, key, (keyCmp)env->type->keyCmp);
    return __hashMapEntryDel(entryBeDel);
}

// HashMapEntry procedures
private hashMapEntry * __hashMapEntryPrev(hashMapEntry *entry) {

}

private hashMapEntry * __hashMapEntryNext(hashMapEntry *entry) {

}

private hashMapEntry * __hashMapEntryTail(hashMapEntry *entry) {

}

private hashMapEntry * __hashMapEntrySearch(hashMapEntry *entry, void *key, keyCmp cmp) {

}

private _Status_t __hashMapEntryInsert(hashMapEntry *entry, hashMapEntry *newEntry, int isAfter) {

}

private _Status_t __hashMapEntryAppend(hashMapEntry *entry, hashMapEntry *newEntry) {

}

private _Status_t __hashMapEntryDel(hashMapEntry *entry) {

    return OK;
}

private hashMapEntry * __hashMapEntryDup(hashMapEntry *entry, void *k_dup, void *v_dup) {
    return null;
}
