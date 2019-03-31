/* hashMap.h */

#include <inttypes.h>
#include "type.h"

/* HashMap(m) ::=
 *   add: m * key * val -> _Status_t
 *   del: m * key -> _Status_t
 *   Search: m * key -> val
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

typedef struct hashMap {
    hashMapEntry **entries; 
    hashMapType *type;
    int size;
} hashMap;

/* Functions implement as macros */
#define hashMapGetSize(HM) ((HM)->size)
#define hashMapGetEntry(HM) ((HM)->entries)

#define hasEntryGetKey(HE) ((HE)->key)
#define hashEntrySetKey(HE, KEY) ((HE)->key = KEY)

#define hashEntryGetValue(HE) ((HE)->value)
#define hashENtrySetValue(HE, VALUE) ((HE)->value = VALUE)

/* Prototype */
hashMap * hashMapCreate(hashMapType *type);
_Status_t hashMapRelease(hashMap *map);
_Status_t hashMapAdd(hashMap *map, const void *key, const void *val);
_Status_t hashMapDel(hashMap *map, const void *key);
void *    hashMapSearch(hashMap *map, const void *key);
hashMap * hashMapDup(hashMap *map);



