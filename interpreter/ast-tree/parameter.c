/* parameter.c */

#include "parameter.h"
#include "wrapper.h"
#include "string.h"
#include "list.h"
#include "expression.h"

/* Private prototypes*/

// Parameter
private _Status_t __paramsRelease(void *);
private _Bool __paramsMatcher(const void *, const void *);
private void * __paramsDup(void *);

// Argument
private _Status_t __argusRelease(void *);
private _Bool __argusMatcher(const void *, const void *);
private void * __argusDup(void *);

/* Public procedures */

// Parameter
Parameters * paramsGen() {
    Parameters *p = (Parameters *)zMalloc(sizeof(Parameters));

    return p;
}

void paramsRelease(Parameters *p) {
    if (p->parameters)
        listRelease(p->parameters);

    free(p);
}


/* Procedure to add parameter call with the first
 * parameter before the second */
_Status_t paramsAdd(Parameters *params, Parameter *param) {
    if (!params->parameters) {
        list *l = listCreate();

        listSetDupMethod(l, __paramsDup);
        listSetMatchMethod(l, __paramsMatcher);
        listSetReleaseMethod(l, __paramsRelease);

        params->parameters = l;
    }

    listAppend(params->parameters, param);
    params->num++;
    return OK;
}

void paramRelease(Parameter *p) {
    if (PARAM_IDENT(p)) free(PARAM_IDENT(p));
    if (PARAM_TYPE(p)) free(PARAM_TYPE(p));
}

Parameter * paramsGetByName(Parameters *p, char *ident) {
    if (!p->parameters) return null;

    listNode *node =  listSearch(p->parameters, ident);

    return (Parameter *)node->value;
}

Parameter * paramsGetByPos(Parameters *p, int pos) {
    if (!p->parameters || p->num < pos) return null;

    listIter iter = listGetIter(p->parameters, LITER_FORWARD);

    while (--pos) {
        iter = listSuccessor(iter);
    }

    return (Parameter*)iter.node->value;
}

Parameter * paramGen(char *ident, char *type) {
    Parameter *p = (Parameter *)zMalloc(sizeof(Parameter));

    PARAM_SET_IDENT(p, ident);
    PARAM_SET_TYPE(p, type);

    return p;
}

// Argument
Arguments * argusGen() {
    return paramsGen();
}

void argusRelease(Arguments *args) {
    if (args->arguments) listRelease(args->arguments);
    free(args);
}

_Status_t argusAdd(Arguments *args, Argument *arg) {
    if (!args->arguments) {
        list *l = listCreate();

        listSetDupMethod(l, );
    }
}

/* Private procedures */

// Parameter
private _Status_t __paramsRelease(void *v) {
    Parameter *p = (Parameter *)v;

    free(PARAM_TYPE(p));
    free(PARAM_IDENT(p));
    free(p);

    return OK;
}

private _Bool __paramsMatcher(const void *v1, const void *v2) {
    Parameter *p1 = (Parameter *)v1;

    return strCompare(PARAM_IDENT(p1), (char *)v2);
}

private void * __paramsDup(void *v) {
    Parameter *orig = (Parameter *)v, *dup;

    dup = paramGen(strdup(PARAM_IDENT(orig)),
                   strdup(PARAM_TYPE(orig)));

    return dup;
}

// Argument
private _Status_t __argusRelease(void *v) {
    Argument *a = (Argument *)v;

}

private _Bool __argusMatcher(const void *v1, const void *v2) {

}

private void * __argusDup(void *v) {

}


#ifdef _AST_TREE_TESTING_

#include "test.h"

void paramTest(void **state) {
    Parameters *params = paramsGen();
    paramsAdd(params, paramGen("a", "Int"));

    // Search by name
    Parameter *param = paramsGetByName(params, "a");
    assert_string_equal(PARAM_IDENT(param), "a");
    assert_string_equal(PARAM_TYPE(param), "Int");

    // Search by pos
    param = paramsGetByPos(params, 1);
    assert_string_equal(PARAM_IDENT(param), "a");
    assert_string_equal(PARAM_TYPE(param), "Int");


}

#endif /* _AST_TREE_TESTING_ */
