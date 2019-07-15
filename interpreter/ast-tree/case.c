/* case.c */

#include "wrapper.h"
#include "case.h"
#include "statement.h"
#include <string.h>

#define FUNC_SET_COMPUTE_ROUTINE(C, COMPUTE) ((C)->compute = COMPUTE)

/* Private prototypes */
private Variable * __funcComputing(Func *c, Scope *s);
private _Status_t __paramsRelease(void *);
private _Bool __paramsMatcher(const void *, const void *);
private void * __paramsDup(void *);

/* Public procedures */
Func * funcGenerate() {
    Func *c = (Func *)zMalloc(sizeof(Func));
    FUNC_SET_COMPUTE_ROUTINE(c, __funcComputing);
    return c;
}

void funcReleae(Func *f) {
    if (FUNC_IDENT(f)) free(FUNC_IDENT(f));
    if (FUNC_STATEMENT_LIST(f)) listRelease(FUNC_STATEMENT_LIST(f));
    if (FUNC_RETURN_TYPE(f)) free(FUNC_RETURN_TYPE(f));
    if (FUNC_PARAMETERS(f)) paramsRelease(FUNC_PARAMETERS(f));
}

/* Procedure to add parameter call with the first
 * parameter before the second */
_Status_t funcAddParam(Func *f, Parameter *p) {
    Parameters *params = f->params;

    paramsAdd(params, p);
    return OK;
}

Parameter *funcGetParamByName(Func *f, char *ident) {
    if (!f->params) return null;
    return paramsGetByName(f->params, ident);
}

/* If you want to get first parameter give i = 1, second
 * parameter give i = 2 and so on. */
Parameter * funcGetParamByPos(Func *f, int i) {
    if (!f->params) return null;
    return paramsGetByPos(f->params, i);
}


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

void paramRelease(Parameter *p) {
    if (PARAM_IDENT(p)) free(PARAM_IDENT(p));
    if (PARAM_TYPE(p)) free(PARAM_TYPE(p));
}

/* Procedure to append statement to function, the statement
 * append earlier will be executed earlier. */
_Status_t funcAppendStatement(Func *c, Statement *s) {
    /* Initialize statements list */
    if (FUNC_IS_EMPTY_FUNC(c)) c->statements = listCreate();

    listAppend(c->statements, s);
    return OK;
}

/* Should provide release method to list. */
_Status_t funcAppendStatements(Func *f, Statement *s) {
    if (!f->statements) f->statements = listCreate();
    return listAppend(f->statements, s);
}

/* Private procedures */

/* This procedure always be called by function call expression
 * compute procedure, local scope is deal with by this procedure. */
private Variable * __funcComputing(Func *c, Scope *s) {
    if (FUNC_IS_EMPTY_FUNC(c)) return null;

    Variable *v;
    StatementTrack st;

    st = statementCompute_untilReturn(c->statements, s);

    if (st.id == RETURN_STATEMENT_ID) {
        v = st.v;
        return v;
    } else {
        /* Every test case should must a
         * return value to indicate whether
         * the test case is running success */
        abortWithMsg("Can't found return in a test case");
    }
}

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

#ifdef _AST_TREE_TESTING_

void funcTest(void **state) {
    Func *f = funcGenerate();

    int a = 1;
    funcAppendStatement(f, returnStmtGen(constExprGen(&a, PRIMITIVE_TYPE_INT)));
}

#endif /* _AST_TREE_TESTING_ */
