/* module.c */

#include "module.h"
#include "func.h"
#include "wrapper.h"
#include "hashTypes.h"

/* Private prototypes */
private Variable * mod_primitiveIntDefine(char *ident, int *val);
private Variable * mod_primitiveStrDefine(char *ident, char *val);
private Variable * mod_objectVarDefine(char *ident, Object *obj);

/* Public procedures  */

// Module info
ModuleInfo * moduleInfoGen(void) {
    return (ModuleInfo *)zMalloc(sizeof(ModuleInfo));
}

// Module
Module * moduleGen(char *name) {
    Module *m = (Module *)zMalloc(sizeof(Module));
    MODULE_SET_NAME(m, name);

    return m;
}

void * modSearchTemplate(Module *m, char *name) {
    ModuleInfo *info = m->info;
    if (!info) return null;

    return hashMapSearch(MODULE_INFO_TEMPLATE(info), name);
}

void * modSearchFunction(Module *m, char *name) {
    ModuleInfo *info = m->info;

    if (!info) return null;
    return hashMapSearch(MODULE_INFO_FUNCTION(info), name);
}

void * modSearchPrimitive(Module *m, char *name) {
    ModuleInfo *info = m->info;

    if (!info) return null;
    return hashMapSearch(MODULE_INFO_PRIMITIVE(info), name);
}

void * modSearchObject(Module *m, char *name) {
    ModuleInfo *info = m->info;

    if (!info) return null;
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
    if (ret) return ret;
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

void * modTblSearchTemplate(ModuleTable *tbl, char *modName, char *tempName) {
    Module *mod = modTblSearchModule(tbl, modName);

    return modSearchTemplate(mod, tempName);
}

void * modTblSearchFunction(ModuleTable *tbl, char *modName, char *funcName) {
    Module *mod = modTblSearchModule(tbl, modName);

    return modSearchFunction(mod, funcName);
}

void * modTblSearchPrimitive(ModuleTable *tbl, char *modName, char *priName) {
    Module *mod = modTblSearchModule(tbl, modName);

    return modSearchPrimitive(mod, priName);
}

void * modTblSearchObject(ModuleTable *tbl, char *modName, char *objName) {
    Module *mod = modTblSearchModule(tbl, modName);

    return modSearchObject(mod, objName);
}

_Status_t modTblAddTemplate(ModuleTable *tbl, char *modName, Template *t) {
    Module *mod = modTblSearchModule(tbl, modName);

    return modAddTemplate(mod, t);
}

_Status_t modTblAddFunction(ModuleTable *tbl, char *modName, Func *f) {
    Module *mod = modTblSearchModule(tbl, modName);

    return modAddFunction(mod, f);
}

_Status_t modTblAddPrimitive(ModuleTable *tbl, char *modName, Variable *v) {
    Module *mod = modTblSearchModule(tbl, modName);

    return modAddPrimitive(mod, v);
}

_Status_t modTblAddObject(ModuleTable *tbl, char *modName, Variable *v) {
    Module *mod = modTblSearchModule(tbl, modName);

    return modAddObject(mod, v);
}

Module * modTblSearchModule(ModuleTable *tbl, char *modName) {
    list *modules = MOD_TBL_LIST(tbl);

    return listSearch_v(modules, modName);
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
        return ERROR;
    }
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

#ifdef _AST_TREE_TESTING_

void moduleTesting(void **state) {}

#endif /* _AST_TREE_TESTING_ */
