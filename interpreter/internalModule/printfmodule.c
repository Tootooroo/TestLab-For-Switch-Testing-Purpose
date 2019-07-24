/* printfmodule.c
 *
 * usage: print("Time is %d:%d" % (hour, min)) */

#include "module.h"
#include "wrapper.h"

/* Private prototypes */
private Variable * print_internal(void *, Scope *);

/* Public procedures */
_Status_t printfModuleInit(void) {
    Func *f = mod_functionDefine("printf", "Int", NULL, NULL, NULL);

    funcAddParam(f, paramGen("str", "String"));
    f->interRtn = print_internal;

    Module *m = moduleGen("printf");
    modAddFunction(m, f);

    modTblAddModule(&moduleTable, m);

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
