/* module.h */

#ifndef _AST_TREE_MODULE_H_
#define _AST_TREE_MODULE_H_

#include "type.h"
#include "list.h"
#include "object.h"
#include "func.h"
#include "hashMap.h"

/* This struct is similar to Scope */
typedef struct ModuleInfo {
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
    /* Map<ident, Template> */
    hashMap *objects;
} ModuleInfo;

typedef struct Module {
    /* Module name
     * Import statment import internal module
     * via module name and module name should
     * be uniquely for each module within mo-
     * dule table.
     * Keyword: (Uniquly, Import) */
    char *name;
    ModuleInfo *info;
} Module;

typedef struct ModuleTable {
    int numOfModules;
    /* list<Module> */
    list *modules;
} ModuleTable;

// Table of module references
ModuleTable moduleTable;
// Identifier of global module
#define GLOBAL_MODULE "GLOBAL"

/* Member functions implement as macros */
#define MODULE_INFO_TEMPLATE(MINFO) ((MINFO)->template)
#define MODULE_INFO_FUNCTION(MINFO) ((MINFO)->functions)
#define MODULE_INFO_PRIMITIVE(MINFO) ((MINFO)->primitives)
#define MODULE_INFO_OBJECT(MINFO) ((MINFO)->objects)

#define MODULE_INFO_SET_TEMPLATE(MINFO, TEMP) ((MINFO)->template = (TEMP))
#define MODULE_INFO_SET_FUNCTION(MINFO, FUNC) ((MINFO)->functions = (FUNC))
#define MODULE_INFO_SET_PRIMITIVE(MINFO, PRI) ((MINFO)->primitives = (PRI))
#define MODULE_INFO_SET_OBJECT(MINFO, OBJ) ((MINFO)->objects = (OBJ))

#define MODULE_NAME(M) ((M)->name)
#define MODULE_SET_NAME(M, N) ((M)->name = (N))

#define MODULE_INFO(M) ((M)->info)
#define MODULE_SET_INFO(M, I) ((m)->INFO = (I))

#define MOD_TBL_RESET_NUM(MT) ((MT)->numOfModules = 0)
#define MOD_TBL_NUM_PLUS(MT) (++(MT)->numOfModules)
#define MOD_TBL_NUM_DESC(MT) (--(MT)->numOfModules)

#define MOD_TBL_LIST(MT) ((MT)->modules)
#define MOD_TBL_SET_MODULES(MT, MODS) ((MT)->modules = (MODS))

/* Prototypes */
_Status_t moduleInit(void);

// Module info
ModuleInfo * moduleInfoGen(void);
void moduleInfoRelease(ModuleInfo *);

// Module
Module * moduleGen(char *name);
void * modSearchTemplate(Module *m, char *name);
void * modSearchFunction(Module *m, char *name);
void * modSearchPrimitive(Module *m, char *name);
void * modSearchObject(Module *m, char *name);
/* Search symbol from Primitives and Objects */
void * modSearchSymbol(Module *m, char *sym);
void moduleRelease(Module *m);

_Status_t modAddTemplate(Module *m, Template *);
_Status_t modAddFunction(Module *m, Func *);
_Status_t modAddPrimitive(Module *, Variable *);
_Status_t modAddObject(Module *, Variable *);

// ModuleTable
ModuleTable * modTblGen(void);
void modTblRelease(ModuleTable *);
void * modTblSearchTemplate(ModuleTable *, char *modName, char *tempName);
void * modTblSearchFunction(ModuleTable *, char *modName, char *funcName);
void * modTblSearchPrimitive(ModuleTable *, char *modName, char *priName);
void * modTblSearchObject(ModuleTable *, char *modName, char *objName);

_Status_t modTblAddTemplate(ModuleTable *, char *modName, Template *);
_Status_t modTblAddFunction(ModuleTable *, char *modName, Func *);
_Status_t modTblAddPrimitive(ModuleTable *, char *modName, Variable *);
_Status_t modTblAddObject(ModuleTable *, char *modName, Variable *);

Module * modTblSearchModule(ModuleTable *, char *modName);
_Status_t modTblAddModule(ModuleTable *, Module *);

// Entity(Variable, function, Object type) define functions
Template * mod_objectDefine(char *typeName, list *members);
/* Type of val :
 * (1) int * if type == VAR_PRIMITIVE_INT
 * (2) char * if type == VAR_PRIMITIVE_STR
 * (3) Object * if type == VAR_OBJECT */
Variable * mod_variableDefine(char *ident, VarType type, void *val);
Func * mod_functionDefine(char *ident, char *type,
                          Parameters *, list *stmts, Scope *s);

#ifdef _AST_TREE_TESTING_

void moduleTesting(void **state);

#endif /* _AST_TREE_TESTING_ */

#endif /* _AST_TREE_MODULE_H_ */
