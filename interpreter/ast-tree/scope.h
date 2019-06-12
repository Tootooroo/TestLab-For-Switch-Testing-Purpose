/* scope.h */

#ifndef _SCOPE_H_
#define _SCOPE_H_

#include "case.h"
#include "object.h"
#include "primitive.h"

#include "hashMap.h"

typedef struct scope {
    /* hashMap of cases */
    hashMap *cases;
    hashMap *primitives;
    hashMap *objects;

    /* Reference to outer scope */
    struct scope *outer;
} scope;

/* Member functions implement as macros */
#define scopeGetCase(s, f) (hashMapSearch(s->cases, (void *)f))
#define scopeGetPrimitive(s, p) (hashMapSearch(s->primitives, (void *)p))
#define scopeGetObject(s, o) (hashMapSearch(s->objects, (void *)o))

#define isCasesExists(s, f) (scopeGetCase(s,f) != null)
#define isPrimitiveExists(s, p) (scopeGetPrimitive(s, p) != null)
#define isObjectExists(s, o) (scopeGetObject(s, o) != null)

/* Prototypes */
scope * scopeGenerate();
scope * subScopeGenerate(scope *);

_Status_t scopeNewCase(scope *, Case *);
_Status_t scopeNewPrimitive(scope *, Primitive *);
_Status_t scopeNewObject(scope *, Object *);

#endif /* _SCOPE_H_ */
