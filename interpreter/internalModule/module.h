/* module.h */

#ifndef _AST_TREE_MODULE_H_
#define _AST_TREE_MODULE_H_

#include "list.h"

typedef struct ModuleInfo {

} ModuleInfo;

typedef struct Module {
    /* Module name
     * Import statment import internal module
     * via module name and module name should
     * be uniquely for each module within mo-
     * dule table.
     * Keyword: (Uniquly, Import)*/
    char *name;
    ModuleInfo *info;
} Module;

typedef struct ModuleTable {
    /* list<Module> */
    list *modules;
} ModuleTable;

#endif /* _AST_TREE_MODULE_H_ */
