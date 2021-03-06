/* scope.h */

#ifndef _SCOPE_H_
#define _SCOPE_H_

#include "pair.h"
#include "hashMap.h"

typedef struct Func Func;
typedef struct Variable Variable;
typedef struct Object Template;
typedef struct ModuleTable ModuleTable;

enum { TEMPLATE_ENTITY, FUNCTION_ENTITY, PRIMITIVE_ENTITY, OBJECT_ENTITY };

typedef struct Scope {
    /* Template is generated by object
     * declaration, new object will be
     * generated by copy from a specific
     * template.
     *
     * Map<type::String, O::Object>*/
    hashMap *template;
    /* Map<ident, function> */
    hashMap *functions;
    /* Map<ident, Variable> */
    hashMap *primitives;
    /* Map<ident, Variable> */
    hashMap *objects;

    /* Global scope is the Head(Top) of scope chain */
    _Bool isGlobal;

    /* For those scope not global use outer
        * pointer to reference to upper scope. */
    struct Scope *outer;
    /* mTbl only used by global scope for the purpose of
        * reference of internal or external module. */
    ModuleTable *mTbl;
} Scope;

#define isFuncExists(s, f) (scopeGetFunc(s,f) != null)
#define isPrimitiveExists(s, p) (scopeGetPrimitive(s, p) != null)
#define isObjectExists(s, o) (scopeGetObject(s, o) != null)
#define isTemplateExists(s, t) (scopeGetTemplate(s, t) != NULL)
#define isGlobalScope(s) ((s)->isGlobal == true)

/* Prototypes */
Scope * scopeGenerate();
Scope * subScopeGenerate(Scope *);
void scopeRelease(Scope *);
void scopeReleaseCurrent(Scope *);
hashMap * scopeGetEntities(Scope *, int);

Func * scopeGetFunc(Scope *, char *);
Variable * scopeGetPrimitive(Scope *, char *);
Variable * scopeGetObject(Scope *, char *);
Template * scopeGetTemplate(Scope *, char *);

_Bool scopeIsValidType(char *type, Scope *);

_Status_t scopeNew(Scope *, pair *, int type);
/* pair<identifier, Func> */
_Status_t scopeNewFunc(Scope *, pair *);
/* pair<identifier, Variable> */
_Status_t scopeNewPrimitive(Scope *, pair *);
/* pair<identifier, Variable> */
_Status_t scopeNewObject(Scope *, pair *);
/* pair<identifier, Template> */
_Status_t scopeNewTemplate(Scope *, pair *);

#ifdef _AST_TREE_TESTING_

void scopeTest(void **state);

#endif /* _AST_TREE_TESTING_ */

#endif /* _SCOPE_H_ */
