/* module.c */

#include "module.h"
#include "func.h"
#include "wrapper.h"
#include "hashTypes.h"

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

    }

    return hashMapAdd(MODULE_INFO_TEMPLATE(info), TEMPLATE_TYPE(t), t);
}

_Status_t modAddFunction(Module *m, Func *f) {
    ModuleInfo *info;
}
_Status_t modAddPrimitive(Module *m, Primitive *p) {}
_Status_t modAddObject(Module *m, Object *o) {}

// Module Table
void * modTblSearchTemplate(ModuleTable *tbl, char *modName, char *tempName) {}
void * modTblSearchFunction(ModuleTable *tbl, char *modName, char *funcname) {}
void * modTblSearchPrimitive(ModuleTable *tbl, char *modName, char *priName) {}
void * modTblSearchObject(ModuleTable *tbl, char *modName, char *objName) {}

_Status_t modTblAddTemplate(ModuleTable *tbl, char *modName, Template *t) {}
_Status_t modTblAddFunction(ModuleTable *tbl, char *modName, Func *f) {}
_Status_t modTblAddPrimitive(ModuleTable *tbl, char *modName, Primitive *p) {}
_Status_t modTblAddObject(ModuleTable *tbl, char *modName, Object *o) {}

Module * modTblSearchModule(ModuleTable *tbl, char *modName) {}
