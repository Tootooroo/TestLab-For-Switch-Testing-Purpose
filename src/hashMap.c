/* hashMap.c */

#include <malloc.h>
#include <string.h>
#include <assert.h>

#include "type.h"
#include "hashMap.h"
#include "wrapper.h"

/* Private variables */

/* if map->maps[0].used / map->maps[0].size > hashMap_force_expand_ratio
 * hashMap will be force to expand and then into rehashing state */
private int hashMap_force_expand_ratio = 5;
private int hashMap_init_size = 256;

/* Private prototypes */
typedef _Bool (*keyCmp)(const void *key);
private hashMapEntry * __hashMapEntryNext(hashMapEntry *entry);
private hashMapEntry * __hashMapEntryTail(hashMapEntry *entry);
private hashMapEntry * __hashMapEntrySearch(hashMapEntry *entry, void *key, keyCmp cmp);
private _Status_t      __hashMapEntryAppend(hashMapEntry *entry, hashMapEntry *newEntry);
private hashMapEntry * __hashMapEntryDup(hashMapEntry *entry, void *k_dup, void *v_dup);
private _Bool          __hashMapEntryIsLastSlot(hashMapEntry **entry, __hashMap *env);
private hashMapEntry * __hashMapEntryDrag(hashMapEntry *entry);

private _Status_t __hashMapRelease_do(hashMapType *tpe, __hashMap *map);
private _Bool __hashMapIsNeedExpand(hashMap *map);
private _Status_t __hashMapExpand(hashMap *map);
private _Status_t __hashMapExpand_do(hashMap *map, int size);
private _Status_t __hashMapRehashingStep(hashMap *map);
private _Status_t __hashMapRehashingSteps(hashMap *map, int N);
private _Status_t __hashMapRehashing(hashMap *map);
private _Status_t __hashMapAdd(__hashMap *map, void *key, void *val);
private _Status_t __hashMapDel(__hashMap *map, void *key, hashMap *env);
/* This function return the first non-null entry of hashmap */
private hashMapEntry * __hashMapFirstEntry(__hashMap *map, hashMapEntry * **slot);

hashMap * hashMapCreate(hashMapType *type) {
    hashMap *map = (hashMap *)zMalloc(sizeof(hashMap));

    map->type = type;
    map->state = IN_NORMAL;
    map->rehashIdx = 0;
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

private hashMapEntry * __hashMapNext(hashMapIter *iter) {
    hashMap *map = iter->map;
    entryPos *pos = &iter->pos;
    hashMapEntry **slot = hashEntryPosSlot(pos), *entry = hashEntryPosEntry(pos);
    int tableIdx = iter->table;

    if (hashMapIsEmpty(map)) return null;
    if (hashMapIsInRehashing(map)) __hashMapRehashingStep(map);

 Again:
    if (!entry) {
        return pos->entry =  __hashMapFirstEntry(&map->maps[tableIdx], &slot);
    }

    /* If still an element is after the current element */
    if (entry->next) {
        return pos->entry = __hashMapEntryNext(entry);
    }

    /* To the next nont-null slot */
    while (hashEntryPosIsEmptySlot(++slot)) {
        if (__hashMapEntryIsLastSlot(slot, &map->maps[tableIdx])) {
            iter->pos.entry = null;
            iter->pos.slot = null;

            if (!hashMapIsInRehashing(map)) {
                iter->table = tableIdx = 1;
                goto Again;
            }
            return null;
        }
    }
    iter->pos.slot = slot;
    entry = *slot;
    iter->pos.entry = entry;;

    return entry;
}

hashMapIter hashMapGetIter(hashMap *map) {
    hashMapIter iter;
    iter.map = map;
    iter.table = 0;
    iter.pos.entry = null;
    iter.pos.slot = null;

    return iter;
}

hashMapIter hashMap_I_Successor(hashMapIter iter) {
    __hashMapNext(&iter);

    return iter;
}

/* Private procedures */

// HashMap procedures
private hashMapEntry * __hashMapFirstEntry(__hashMap *map, hashMapEntry * **slot) {
    hashMapEntry **slot_;

    slot_ = map->entries;

    if (slot_) return slot_[0];

    while (!(++slot_)[0]) {
        if (__hashMapEntryIsLastSlot(slot_, map))
            return null;
    }

    *slot = slot_;
    return slot_[0];
}

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

private _Bool __hashMapIsNeedExpand(hashMap *map) {
    if (hashMapIsInRehashing(map)) return false;

    if (hashMapIsEmpty(map) ||
        map->maps[0].used / map->maps[0].size > hashMap_force_expand_ratio)

        return true;

    return false;
}

/* __hashMapIsNeedExpand should be call first to detect that
 * expand condition has been satisfied */
private _Status_t __hashMapExpand(hashMap *map) {
    if (hashMapIsEmpty(map)) {
        /* Expand hashMap to default size */
        __hashMapExpand_do(map, hashMap_init_size);
    } else {
        /* Expand hashMap to 2 * map->maps[0].size size */
        __hashMapExpand_do(map, map->maps[0].size);
    }
    return OK;
}

private _Status_t __hashMapExpand_do(hashMap *map, int size) {
    __hashMap *theMap = &map->maps[1];

    theMap->size = size;
    theMap->used = 0;

    theMap->entries = (hashMapEntry **)zMalloc(size * (sizeof(hashMapEntry *)));
    theMap->last = theMap->entries + (size - 1);

    map->state = IN_RE_HASHING;
    map->rehashIdx = 0;

    return OK;
}

/* Don only a step of rehashing */
private _Status_t __hashMapRehashingStep(hashMap *map) {
    return OK;
}

/* Do N steps of rehashing */

/* This macro should update map->rehashIdx */
#define __REHASHING_THE_SLOT(M) ({       \
    int idx = M->rehashIdx, ret = OK;                                \
    hashMapEntry **currentEntry = &M->maps[0].entries[idx];\
    while (!currentEntry) {\
        if (__hashMapEntryIsLastSlot(currentEntry)) {   \
            ret = ERROR;                                \
            goto EXIT_THE_SLOT;                         \
        }                                               \
        ++currentEntry;\
        ++idx;\
    }\
EXIT_THE_SLOT:\
    M->rehashIdx = idx;
    ret;      \
})

#define __REHASHING_STEPS(N_, M) ({\
    int idx, ret = OK;\
    hashMapEntry *current, *next, **newSlot;\
    \
    while (--N_) {\
        current = __REHASHING_THE_SLOT(M);   \
        \
        if (isNull(current)) { ret = ERROR; goto EXIT; }\
        \
        while (current) {\
            next = current->next;\
            \
            idx = M->type->hashing(current->key) & M->maps[1].size;\
            newSlot = &M->maps[1].entries[idx];\
            current = __hashMapEntryDrag(current);\
            if (*newSlot == null)\
                *newSlot = current;                       \
            else\
                __hashMapEntryAppend(*newSlot, current);    \
            \
            current = next;\
        }                  \
    }\
EXIT:\
    ret;\
})

private _Status_t __hashMapRehashingSteps(hashMap *map, int N) {
    assert(map->maps[0].size > map->rehashIdx);

    /* Do rehashing with N steps */
    __REHASHING_STEPS(N, map);

    /* Is rehashing finished ? */
    if (map->maps[0].used == 0) {
        map->state = IN_NORMAL;
        map->rehashIdx = 0;

        free(map->maps[0].entries);
        map->maps[0] = map->maps[1];
    }

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
    hashMapEntry *entryHead, *entryBeDel, *entryPrev;

    entryHead = map->entries[index];

    /* Deal with the first element of the slot */
    if (env->type->keyCmp(key, entryHead->key)) {
        env->type->keyRelease(entryHead->key);
        env->type->valRelease(entryHead->value);

        map->entries[index] = entryHead->next;

        free(entryHead);

        return OK;
    }

    entryPrev = entryHead;
    while (entryHead = __hashMapEntryNext(entryHead)) {
        if (env->type->keyCmp(key, entryHead->key)) {
            env->type->keyRelease(entryHead->key);
            env->type->valRelease(entryHead->value);

            entryPrev->next = entryHead->next;

            free(entryHead);
            return OK;
        }

        entryPrev = entryHead;
    }

    return ERROR;
}

// HashMapEntry procedures
private _Bool __hashMapEntryIsLastSlot(hashMapEntry **entry, __hashMap *env) {
    return env->last == entry;
}

private hashMapEntry * __hashMapEntryNext(hashMapEntry *entry) {
    return entry->next;
}

private hashMapEntry * __hashMapEntryTail(hashMapEntry *entry) {
    while (entry) {
        if (entry->next == NULL)
            break;
        entry = __hashMapEntryNext(entry);
    }
    return entry;
}

private hashMapEntry * __hashMapEntrySearch(hashMapEntry *entry, void *key, keyCmp cmp) {
    while (entry) {
        if (cmp(entry->key, key))
            break;
        entry = __hashMapEntryNext(entry);
    }

    return entry;
}

private _Status_t __hashMapEntryAppend(hashMapEntry *entry, hashMapEntry *newEntry) {
    entry = __hashMapEntryTail(entry);

    entry->next = newEntry;

    return OK;
}

private hashMapEntry * __hashMapEntryDup(hashMapEntry *entry, void *k_dup, void *v_dup) {
    hashMapEntry *entry = (hashMapEntry *)zMalloc(sizeof(hashMapEntry));
    return null;
}

private hashMapEntry * __hashMapEntryDrag(hashMapEntry *entry) {
    return null;
}
