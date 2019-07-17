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
    if (PARAM_IDENT(p)) free(PAIR_GET_LEFT(p->p));
    if (PARAM_TYPE(p)) free(PAIR_GET_RIGHT(p->p));
    free(p);
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
    p->p = pairGen(null, null);

    PARAM_SET_IDENT(p, ident);
    PARAM_SET_TYPE(p, type);

    return p;
}

// Argument
Arguments * argusGen() {
    return (Arguments *)zMalloc(sizeof(Arguments));
}

void argusRelease(Arguments *args) {
    if (args->arguments) listRelease(args->arguments);
    free(args);
}

_Status_t argusAdd(Arguments *args, Argument *arg) {
    if (!args->arguments) {
        list *l = listCreate();

        listSetReleaseMethod(l, __argusRelease);
        /* Argument can only search by position */
        listSetMatchMethod(l, NULL);
        listSetDupMethod(l, __argusDup);

        args->arguments = l;
    }

    listAppend(args->arguments, arg);
    ++args->num;

    return OK;
}

/* If you want to get first parameter give i = 1, second
 * parameter give i = 2 and so on. */
Argument * argusGetByPos(Arguments *args, int i) {
    if (!args->arguments || args->num < i) return null;

    listIter iter = listGetIter(args->arguments, LITER_FORWARD);

    while (--i) {
        iter = listSuccessor(iter);
    }
    return (Argument *)iter.node->value;
}

Argument * arguGen(Expression *expr) {
    Argument *arg = (Argument *)zMalloc(sizeof(Argument));

    ARGU_SET_EXPR(arg, expr);

    return arg;
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

    // fixme: Expression's release member has not be initialized.
    //Expression *expr = ARGU_EXPR(a);
    //expr->release(expr, a->outer);

    return OK;
}

/* Caution: This proc do shallow duplication should
 * never be used. */
private void * __argusDup(void *v) {
    Argument *orig = (Argument *)v,
        *dup = arguGen(ARGU_EXPR(orig));

    return dup;
}

_Status_t argusStore(Arguments *args, Parameters *params, Scope *scope) {
    /* Number of parameters if not equal to number of arguments */
    int paramNum = PARAM_NUM(params);

    if (paramNum != ARGU_NUM(args))
        return ERROR;
    else {
        /* Empty parameter list and argument list */
        if (paramNum == 0)
            return OK;
    }

    listIter params_iter = listGetIter(params->parameters, LITER_FORWARD),
        args_iter = listGetIter(args->arguments, LITER_FORWARD);

    while (paramNum--) {
        listNode *param_node = listNext(&params_iter);
        listNode *arg_node = listNext(&args_iter);

        Parameter *param = (Parameter *)param_node->value;
        Argument  *argu  = (Argument  *)arg_node->value;

        Expression *expr = ARGU_EXPR(argu);

        Variable *var = expr->compute(expr, scope);

        /* Type check */
        if (varIsType(var, PARAM_TYPE(param)) == false) {
            /* Not match */
            return ERROR;
        }

        /* Store into scope */
        var->iOps->pass(var, scope, PARAM_IDENT(param));
    }

    return OK;
}

#ifdef _AST_TREE_TESTING_

#include "test.h"

void paramTest(void **state) {

    /* Parameter Testing */
    Parameters *params = paramsGen();
    paramsAdd(params, paramGen(strdup("a"), strdup("Int")));

    // Search by name
    Parameter *param = paramsGetByName(params, "a");
    assert_non_null(param);
    assert_string_equal(PARAM_IDENT(param), "a");
    assert_string_equal(PARAM_TYPE(param), "Int");

    // Search by pos
    param = paramsGetByPos(params, 1);
    assert_non_null(param);
    assert_string_equal(PARAM_IDENT(param), "a");
    assert_string_equal(PARAM_TYPE(param), "Int");

    paramsRelease(params);

    /* Argument Testing */
    Arguments *argus = argusGen();
    argusAdd(argus, arguGen((Expression *)constExprGen("123", PRIMITIVE_TYPE_STR)));

    // Search by pos
    Argument *argu = argusGetByPos(argus, 1);
    assert_non_null(argu);

    ConstantExpression *cExpr = (ConstantExpression *)argu->expr;
    Variable *var = cExpr->constant_var;
    assert_int_equal(varIsType(var, "String"), true);
    assert_string_equal(getPrimitive_str(var->p), "123");

    argusRelease(argus);
}

#endif /* _AST_TREE_TESTING_ */
