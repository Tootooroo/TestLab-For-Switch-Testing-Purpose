/* case.c */

#include "wrapper.h"
#include "func.h"
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

Func * funcGen(char *ident, char *type, Parameters *params, Scope *s) {
    Func *f_def = funcGenerate();

    FUNC_SET_IDENT(f_def, ident);
    FUNC_SET_RETURN_TYPE(f_def, type);

    if (s)
        f_def->outer = s;
    else
        f_def->outer = scopeGenerate();

    if (params)
        f_def->params = params;
    else
        f_def->params = paramsGen();

    return f_def;
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
