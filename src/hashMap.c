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
    
    map->size = 0;
    map->type = type;
    map->entries = NULL;

    return map;
}

_Status_t hashMapRelease(hashMap *map) {
      
}

_Status_t hashMapAdd(hashMap *map, const void *key, const void *val) {

}

_Status_t hashMapDel(hashMap *map, const void *key) {

}

void * hashMapSearch(hashMap *map, const void *key) {

}

hashMap * hashMapDup(hashMap *map) {

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

