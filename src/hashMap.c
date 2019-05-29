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
private hashMapEntry * __hashMapEntryNext(hashMapEntry *entry);
private hashMapEntry * __hashMapEntryTail(hashMapEntry *entry);
private hashMapEntry * __hashMapEntrySearch(hashMapEntry *entry, void *key, hashMap *map);
private _Status_t      __hashMapEntryAppend(hashMapEntry *entry, hashMapEntry *newEntry);
private hashMapEntry * __hashMapEntryDup(hashMapEntry *entry, hashMap *env);
private _Bool          __hashMapEntryIsLastSlot(hashMapEntry **entry, __hashMap *env);
private hashMapEntry * __hashMapEntryDrag(hashMapEntry *entry);

private _Status_t __hashMapRelease_do(hashMapType *tpe, __hashMap *map);
private _Bool __hashMapIsNeedExpand(hashMap *map);
private _Status_t __hashMapExpand(hashMap *map);
private _Status_t __hashMapExpand_do(hashMap *map, int size);
private _Status_t __hashMapRehashingStep(hashMap *map);
private _Status_t __hashMapRehashingSteps(hashMap *map, int N);
private _Status_t __hashMapRehashing(hashMap *map);
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
        free(&map->maps[1]);
    }

    __hashMapRelease_do(map->type, &map->maps[0]);
    free(&map->maps[0]);

    return OK;
}

_Status_t hashMapAdd(hashMap *map, void *key, void *val) {
    __hashMap *currentMap;

    if (__hashMapIsNeedExpand(map)) __hashMapExpand(map);
    if (hashMapIsInRehashing(map)) __hashMapRehashingStep(map);

    if (hashMapIsInRehashing(map))
        currentMap = &map->maps[1];
    else
        currentMap = &map->maps[0];

    uint64_t hashIdx = hashMapHash(map, key) & hashMapSizeMask(currentMap->size);
    hashMapEntry *newEntry = (hashMapEntry *)zMalloc(sizeof(hashMapEntry));
    hashEntrySetKey(newEntry, key);
    hashEntrySetValue(newEntry, val);

    newEntry->next = currentMap->entries[hashIdx];
    currentMap->entries[hashIdx] = newEntry;

    currentMap->used++;

    return OK;
}

_Status_t hashMapDel(hashMap *map, void *key) {
    if (hashMapIsEmpty(map)) return OK;
    if (hashMapIsInRehashing(map)) __hashMapRehashingStep(map);

    if (__hashMapDel(&map->maps[0], key, map) == OK)
        return OK;

    /* If we can't find such element then try to search
     * the element in slave map */
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

        index = index_ & hashMapSizeMask(currentMap->size);
        entry = currentMap->entries[index];

        if (entry == null)
            break;

        do {
            if (map->type->keyCmp(key, entry->key))
                return entry->value;
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
            if (subMap->entries[idx] == null) {
                idx++;
                continue;
            }

            map_dup->maps[i].entries[idx] = __hashMapEntryDup(subMap[i].entries[idx], map);
            idx++;
        }
        if (!hashMapIsInRehashing(map)) break;
    }
    return map_dup;
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

    if (slot_[0]) {
        *slot = slot_;
        return slot_[0];
    }

    while (!(++slot_)[0]) {
        if (__hashMapEntryIsLastSlot(slot_, map))
            return null;
    }

    *slot = slot_;
    return slot_[0];
}

private _Status_t __hashMapRelease_do(hashMapType *type, __hashMap *map) {
    hashMapEntry **entries, *currentEntry, *nextEntry;

    for (entries = map->entries; entries != map->last; ++entries) {
        if (*entries == null)
            continue;

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

    if (map->maps[0].size == 0)
        return true;

    int ratio = map->maps[0].used / map->maps[0].size;
    if (ratio > hashMap_force_expand_ratio)
        return true;

    return false;
}

/* __hashMapIsNeedExpand should be call first to detect that
 * expand condition has been satisfied */
private _Status_t __hashMapExpand(hashMap *map) {
    if (map->maps[0].size == 0) {
        /* Expand hashMap to default size */
        __hashMapExpand_do(map, hashMap_init_size);
    } else {
        /* Expand hashMap to 2 * map->maps[0].size size */
        __hashMapExpand_do(map, map->maps[0].size * 2);
    }
    return OK;
}

private _Status_t __hashMapExpand_do(hashMap *map, int size) {
    __hashMap *theMap = &map->maps[1];

    if (map->maps[0].size == 0) {
        theMap = &map->maps[0];
    } else {
        map->state = IN_RE_HASHING;
        map->rehashIdx = 0;
    }

    theMap->size = size;
    theMap->used = 0;

    theMap->entries = (hashMapEntry **)zMalloc(size * (sizeof(hashMapEntry *)));
    theMap->last = &theMap->entries[size-1];

    return OK;
}

/* Don only a step of rehashing */
private _Status_t __hashMapRehashingStep(hashMap *map) {
    return __hashMapRehashingSteps(map, 1);
}

/* Do N steps of rehashing */

/* This macro should update map->rehashIdx */
hashMapEntry * __REHASHING_THE_SLOT(hashMap *M) {
    int idx = M->rehashIdx;
    hashMapEntry *ret, **currentEntry = &M->maps[0].entries[idx];
    while (!(*currentEntry)) {
        if (__hashMapEntryIsLastSlot(currentEntry, &M->maps[0])) {
            ret = null;
            goto EXIT_THE_SLOT;
        }
        ++currentEntry;
        ++idx;
    }
    ret = *currentEntry;
EXIT_THE_SLOT:
    ++idx;
    M->rehashIdx = idx;
    return ret;
}

private _Status_t __REHASHING_STEPS(int N, hashMap *M) {
    int idx, ret = OK;
    hashMapEntry *current, *next, **newSlot;

    while (N--) {
        current = __REHASHING_THE_SLOT(M);
        /* No more slot to be rehashed */
        if (isNull(current)) { ret = OK; goto EXIT; }

        while (current) {
            next = current->next;
            idx = hashMapHash(M, current->key) & hashMapSizeMask(M->maps[1].size);
            newSlot = &M->maps[1].entries[idx];
            current = __hashMapEntryDrag(current);
            if (*newSlot == null)
                *newSlot = current;
            else
                __hashMapEntryAppend(*newSlot, current);

            current = next;
            M->maps[0].used--;
            M->maps[1].used++;
        }
    }
EXIT:
    return ret;
}

private _Status_t __hashMapRehashingSteps(hashMap *map, int N) {

    if (!hashMapIsInRehashing(map)) return ERROR;

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
    int steps = map->maps[0].size;

    return __hashMapRehashingSteps(map, steps);
}

private _Status_t __hashMapDel(__hashMap *map, void *key, hashMap *env) {
    uint64_t index = hashMapHash(env, key) & hashMapSizeMask(map->size);
    hashMapEntry *entryHead, *entryPrev, *entryCurrent;

    entryHead = map->entries[index];
    entryCurrent = entryHead;

    entryPrev = entryHead;
    do {
        if (env->type->keyCmp(key, entryCurrent->key)) {
            env->type->keyRelease(entryCurrent->key);
            env->type->valRelease(entryCurrent->value);

            if (entryCurrent == entryHead) {
                map->entries[index] = entryCurrent->next;
            } else {
                entryPrev->next = entryCurrent->next;
            }

            map->used--;
            return OK;
        }
        entryPrev = entryCurrent;
    } while (entryCurrent = __hashMapEntryNext(entryCurrent));

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

private hashMapEntry * __hashMapEntrySearch(hashMapEntry *entry, void *key, hashMap *map) {
    while (entry) {
        if (map->type->keyCmp(key, entry->key))
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

private hashMapEntry * __hashMapEntryDup(hashMapEntry *entry, hashMap *map) {
    hashMapEntry anchor = { null, null, null }, *dupEntry;
    hashMapType *env = map->type;

    do {
        dupEntry = (hashMapEntry *)zMalloc(sizeof(hashMapEntry));
        dupEntry->key = env->keyDup(entry->key);
        dupEntry->value = env->valDup(entry->value);
        __hashMapEntryAppend(&anchor, dupEntry);
    } while (entry = __hashMapEntryNext(entry));

    return anchor.next;
}

private hashMapEntry * __hashMapEntryDrag(hashMapEntry *entry) {
    entry->next = null;
    return entry;
}

#ifdef _TEST_LAB_UNIT_TESTING_

#include "test.h"

uint64_t testHashing(const void *key) {
    const int *key_int = (const int *)key;

    return *key_int;
}

_Status_t testKeyRelease(void *key) {
    free(key);
    return OK;
}

void * testKeyDup(void *key) {
    int *key_int = (int *)key;
    int *new_key = (int *)zMalloc(sizeof(int));

    *new_key = *key_int;
    return new_key;
}

_Bool testKeyCmp(void *key1, void *key2) {
    int *key_int1, *key_int2;

    key_int1 = (int *)key1;
    key_int2 = (int *)key2;

    return *key_int1 == *key_int2;
}

_Status_t testValRelease(void *val) {
    free(val);
    return OK;
}

void * testValDup(void *val) {
    int *val_int = (int *)val;
    int *new_val = (int *)zMalloc(sizeof(int));

    *new_val = *val_int;

    return new_val;
}

hashMapType testType = {
    testHashing,
    testKeyRelease,
    testKeyDup,
    testKeyCmp,
    testValRelease,
    testValDup
};

void hashMap_Basic(void **state) {
    hashMap *map = hashMapCreate(&testType);

    int *a = (int *)zMalloc(sizeof(int)),
        *a1 = (int *)zMalloc(sizeof(int));
    *a = 0, *a1 = 2;
    hashMapAdd(map, (void *)a, (void *)a1);
    int *pa = hashMapSearch(map, (void *)a);
    assert_int_equal(*pa, *a1);

    hashMapDel(map, (void *)a);

    int a_int = 0;
    assert_non_null(!hashMapSearch(map, (void *)&a_int));

    /* Insert much of elements to trigger expand */
    const int num_of_elem_to_insert = map->maps[0].size * 100;
    int idx = 0, count = num_of_elem_to_insert;

    while (count--) {
        a = (int *)zMalloc(sizeof(int));
        a1 = (int *)zMalloc(sizeof(int));

        *a = idx, *a1 = idx + 1;
        hashMapAdd(map, (void *)a, (void *)a1);
        idx++;
    }

    idx = 0;
    count = num_of_elem_to_insert;
    while (count--) {
        a1 = (int *)hashMapSearch(map, (void *)&idx);
        assert_int_equal(*a1, idx + 1);
    }

    /* HashMap dup */
    hashMap *map1 = hashMapDup(map);
    idx = 0;
    count = num_of_elem_to_insert;
    while (count--) {
        a1 = (int *)hashMapSearch(map1, (void *)&idx);
        assert_int_equal(*a1, idx + 1);
    }

    /* Release HashMap */
    hashMapRelease(map);
}

#endif
