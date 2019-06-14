/* scope.h */

#ifndef _SCOPE_H_
#define _SCOPE_H_

#include "pair.h"
#include "hashMap.h"

typedef struct Scope {
    /* hashMap of cases */
    hashMap *cases;
    hashMap *primitives;
    hashMap *objects;

    /* Reference to outer scope */
    struct Scope *outer;
} Scope;

/* Member functions implement as macros */
#define scopeGetCase(s, f) (hashMapSearch(s->cases, (void *)f))
#define scopeGetPrimitive(s, p) (hashMapSearch(s->primitives, (void *)p))
#define scopeGetObject(s, o) (hashMapSearch(s->objects, (void *)o))

#define isCasesExists(s, f) (scopeGetCase(s,f) != null)
#define isPrimitiveExists(s, p) (scopeGetPrimitive(s, p) != null)
#define isObjectExists(s, o) (scopeGetObject(s, o) != null)

/* Prototypes */
Scope * scopeGenerate();
Scope * subScopeGenerate(Scope *);
_Status_t scopeRelease(Scope *);

_Status_t scopeNewCase(Scope *, pair *);
_Status_t scopeNewPrimitive(Scope *, pair *);
_Status_t scopeNewObject(Scope *, pair *);

#endif /* _SCOPE_H_ */
