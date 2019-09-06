/* module.c */

#include "module.h"
#include "func.h"
#include "wrapper.h"
#include "hashTypes.h"
#include "statement.h"

#include <string.h>

#include "module_extern.h"

typedef _Status_t (*moduleInitRtn)(void);

/* Public variables */
moduleInitRtn moduleInitRtnArray[] = {
    /* printf */
    basicModuleInit,
    NULL
};

ModuleTable moduleTable;

/* Private prototypes */
private Variable * mod_primitiveIntDefine(char *ident, int *val);
private Variable * mod_primitiveStrDefine(char *ident, char *val);
private Variable * mod_objectVarDefine(char *ident, Object *obj);

private _Bool moduleMatch(const void *l_mod, const void *r_modName);

/* Public procedures  */
_Status_t moduleInit(void) {
    moduleInitRtn *currentRtn = moduleInitRtnArray;
    if (currentRtn == NULL) return ERROR;

    memset(&moduleTable, 0, sizeof(ModuleTable));

    while (*currentRtn) { (*currentRtn)(); ++currentRtn; }

    return OK;
}

// Module info
ModuleInfo * moduleInfoGen(void) {
    return (ModuleInfo *)zMalloc(sizeof(ModuleInfo));
}

void moduleInfoRelease(ModuleInfo *info) {}

// Module
Module * moduleGen(char *name) {
    Module *m = (Module *)zMalloc(sizeof(Module));
    MODULE_SET_NAME(m, name);

    return m;
}

void moduleRelease(Module *m) {}

void * modSearchTemplate(Module *m, char *name) {
    ModuleInfo *info = m->info;

    if (!info) return null;

    hashMap *map = MODULE_INFO_TEMPLATE(info);
    if (!map) return NULL;

    return hashMapSearch(MODULE_INFO_TEMPLATE(info), name);
}

void * modSearchFunction(Module *m, char *name) {
    ModuleInfo *info = m->info;

    if (!info) return null;

    hashMap *map = MODULE_INFO_FUNCTION(info);
    if (!map) return NULL;

    return hashMapSearch(MODULE_INFO_FUNCTION(info), name);
}

void * modSearchPrimitive(Module *m, char *name) {
    ModuleInfo *info = m->info;

    if (!info) return null;

    hashMap *map = MODULE_INFO_PRIMITIVE(info);
    if (map == NULL) return NULL;

    return hashMapSearch(map, name);
}

void * modSearchObject(Module *m, char *name) {
    ModuleInfo *info = m->info;

    if (!info) return null;
    hashMap *map = MODULE_INFO_OBJECT(info);
    if (!map) return NULL;

    return hashMapSearch(MODULE_INFO_OBJECT(info), name);
}

void * modSearchSymbol(Module *m, char *sym) {
    ModuleInfo *info = m->info;
    if (!info) return null;

    void *ret = null;

    ret = hashMapSearch(MODULE_INFO_PRIMITIVE(info), sym);
    if (ret) return ret;

    ret = hashMapSearch(MODULE_INFO_OBJECT(info), sym);
    if (ret) return ret;

    ret = hashMapSearch(MODULE_INFO_FUNCTION(info), sym);
    if (ret) return ret;

    ret = hashMapSearch(MODULE_INFO_TEMPLATE(info), sym);

    return ret;
}

_Status_t modAddTemplate(Module *m, Template *t) {
    ModuleInfo *info = m->info;
    if (!info) {
        info = moduleInfoGen();

        MODULE_INFO_SET_TEMPLATE(info, hashMapCreate(&scopeTypeVar));
        m->info = info;
    }

    return hashMapAdd(MODULE_INFO_TEMPLATE(info), TEMPLATE_TYPE(t), t);
}

_Status_t modAddFunction(Module *m, Func *f) {
    ModuleInfo *info = m->info;
    if (!info) {
        info = moduleInfoGen();

        MODULE_INFO_SET_FUNCTION(info, hashMapCreate(&scopeTypeVar));
        m->info = info;
    }

    return hashMapAdd(MODULE_INFO_FUNCTION(info), FUNC_IDENT(f), f);
}
_Status_t modAddPrimitive(Module *m, Variable *v) {
    ModuleInfo *info = m->info;
    if (!info) {
        info = moduleInfoGen();

        MODULE_INFO_SET_PRIMITIVE(info, hashMapCreate(&scopeTypeVar));
        m->info = info;
    }

    return hashMapAdd(MODULE_INFO_PRIMITIVE(info), VAR_IDENT(v), v);
}

_Status_t modAddObject(Module *m, Variable *v) {
    ModuleInfo *info = m->info;
    if (!info) {
        info = moduleInfoGen();

        MODULE_INFO_SET_OBJECT(info, hashMapCreate(&scopeTypeVar));
        m->info = info;
    }

    return hashMapAdd(MODULE_INFO_OBJECT(info), VAR_IDENT(v), v);
}

// Module Table
ModuleTable * modTblGen(void) {
    ModuleTable *modTbl = (ModuleTable *)zMalloc(sizeof(ModuleTable));

    return modTbl;
}

void modTblRelease(ModuleTable *tbl) {}

#define SEARCH_MODULE(T, M) ({\
    void *v;\
    if ((M) == NULL) v = modTblSearchModule((T), GLOBAL_MODULE);\
    else v = modTblSearchModule((T), (M));\
    v;\
})

void * modTblSearchTemplate(ModuleTable *tbl, char *modName, char *tempName) {
    return modSearchTemplate(SEARCH_MODULE(tbl, modName), tempName);
}

void * modTblSearchFunction(ModuleTable *tbl, char *modName, char *funcName) {
    return modSearchFunction(SEARCH_MODULE(tbl, modName), funcName);
}

void * modTblSearchPrimitive(ModuleTable *tbl, char *modName, char *priName) {
    return modSearchPrimitive(SEARCH_MODULE(tbl, modName), priName);
}

void * modTblSearchObject(ModuleTable *tbl, char *modName, char *objName) {
    return modSearchObject(SEARCH_MODULE(tbl, modName), objName);
}

_Status_t modTblAddTemplate(ModuleTable *tbl, char *modName, Template *t) {
    return modAddTemplate(SEARCH_MODULE(tbl, modName), t);
}

_Status_t modTblAddFunction(ModuleTable *tbl, char *modName, Func *f) {
    return modAddFunction(SEARCH_MODULE(tbl, modName), f);
}

_Status_t modTblAddPrimitive(ModuleTable *tbl, char *modName, Variable *v) {
    return modAddPrimitive(SEARCH_MODULE(tbl, modName), v);
}

_Status_t modTblAddObject(ModuleTable *tbl, char *modName, Variable *v) {
    return modAddObject(SEARCH_MODULE(tbl, modName), v);
}

Module * modTblSearchModule(ModuleTable *tbl, char *modName) {
    return listSearch_v(MOD_TBL_LIST(tbl), modName);
}

_Status_t modTblAddModule(ModuleTable *tbl, Module *mod) {
    if (!MOD_TBL_LIST(tbl)) {
        /* Initialize module list */
        list *l_ = listCreate();

        listSetMatchMethod(l_, moduleMatch);
        listSetReleaseMethod(l_, moduleRelease);

        tbl->modules = l_;
    }
    listAppend(MOD_TBL_LIST(tbl), mod);
}

// Entity define functions
Variable * mod_variableDefine(char *ident, VarType type, void *val) {
    switch (type) {
    case VAR_PRIMITIVE_INT:
        return mod_primitiveIntDefine(ident, val);
    case VAR_PRIMITIVE_STR:
        return mod_primitiveStrDefine(ident, val);
    case VAR_OBJECT:
        return mod_objectVarDefine(ident, val);
    default:
        return NULL;
    }
}

Template * mod_objectDefine(char *typeName, list *members) {
    Template *temp = templateGen(typeName);

    TEMPLATE_SET_MEMBERS(temp, members);

    return temp;
}

Func * mod_functionDefine(char *ident, typeInfo *type,
                          Parameters *params,
                          list *stmts, Scope *s) {
    Func *func = funcGen(ident, type, params, s);
    FUNC_SET_STATEMENTS(func, stmts);

    return func;
}

/* Private procedures */
private Variable * mod_primitiveIntDefine(char *ident, int *val) {
    Variable *var = varGen(ident, VAR_PRIMITIVE_INT, val);

    return var;
}

private Variable * mod_primitiveStrDefine(char *ident, char *val) {
    Variable *var = varGen(ident, VAR_PRIMITIVE_STR, val);

    return var;
}

private Variable * mod_objectVarDefine(char *ident, Object *obj) {
    Variable *var = varGen(ident, VAR_OBJECT, obj);

    return var;
}

private _Bool moduleMatch(const void *l_mod, const void *r_modName) {
    Module *mod = (Module *)l_mod;
    char *modName = (char *)r_modName;

    return strCompare(MODULE_NAME(mod), modName);
}

#ifdef _AST_TREE_TESTING_

#include "test.h"

/* Build a module then deploy and call */
void moduleTesting(void **state) {
    ModuleTable *tbl = modTblGen();

    Func *f = funcGen("fun", NULL, NULL, NULL);

    Module *m = moduleGen(GLOBAL_MODULE);

    modAddFunction(m, f);

    modTblAddModule(tbl, m);

    Func *f_found = modTblSearchFunction(tbl, GLOBAL_MODULE, "fun");
    assert_non_null(f_found);
    assert_int_equal(strCompare(FUNC_IDENT(f_found), "fun"), true);
}

#endif /* _AST_TREE_TESTING_ */
