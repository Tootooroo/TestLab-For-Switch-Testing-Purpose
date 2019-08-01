/* basicModule.c
 *
 * basicModule be installled during initialization
 *
 * All entities of this module will be installed into
 * global area of module table so you can reference to
 * these enetity without module name prefix.
 *
 * print("Time is %d:%d" % (hour, min))
 */

#include "module.h"
#include "wrapper.h"

#include "program.h"

/* Private prototypes */
private Variable * print_internal(void *, Scope *);
private void printInit(void);

/* Public procedures */
_Status_t basicModuleInit() {

    // Printf
    printInit();

    return OK;
}

/* Private procedures */
private Variable * print_internal(void *f_void, Scope *s) {
    Func *f = (Func *)f_void;

    Parameter *param = funcGetParamByPos(f, 1);
    if (param == NULL) return NULL;

    Variable *v = scopeGetPrimitive(s, PARAM_IDENT(param));
    char *str = VAR_GET_PRIMITIVE_STR(v);

    if (str == NULL) return NULL;

    printf(str);

    return varGen(NULL, VAR_PRIMITIVE_INT, NULL);
}

private void printInit(void) {
    Func *f = mod_functionDefine("print", "Int", NULL, NULL, NULL);

    FUNC_SET_TYPE(f, FUNC_INTERNAL);

    funcAddParam(f, paramGen("str", "String"));

    f->interRtn = print_internal;
    f->outer = NULL;

    Module *m = moduleGen(GLOBAL_MODULE);
    modAddFunction(m, f);

    modTblAddModule(&moduleTable, m);
}

