/* case.c */

#include "wrapper.h"
#include "case.h"
#include "statement.h"

#define FUNC_SET_COMPUTE_ROUTINE(C, COMPUTE) ((C)->compute = COMPUTE)

/* Private prototypes */
private Variable * __funcComputing(Func *c, Scope *s);

/* Public procedures */
Func * funcGenerate() {
    Func *c = (Func *)zMalloc(sizeof(Func));
    FUNC_SET_COMPUTE_ROUTINE(c, __funcComputing);
    return c;
}

_Status_t caseAppendStatement(Func *c, Statement *s) {
    /* Initialize statements list */
    if (FUNC_IS_EMPTY_FUNC(c)) c->statements = listCreate();

    listAppend(c->statements, s);
    return OK;
}

/* Private procedures */
private Variable * __funcComputing(Func *c, Scope *s) {
    if (FUNC_IS_EMPTY_FUNC(c)) return null;

    Variable *v;
    StatementTrack st;
    Scope *localScope = subScopeGenerate(s);

    st = statementCompute_untilReturn(c->statements, localScope);

    if (st.id == RETURN_STATEMENT_ID) {
        v = st.v;
        scopeRelease(localScope);

        return v;
    } else {
        /* Every test case should have a bool return value to indicate whether
        * the test case is running success */
        abortWithMsg("Can't found return in a test case");
    }
}
