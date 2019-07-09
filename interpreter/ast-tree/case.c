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

/* Should provide release method to list. */
_Status_t funcAppendStatements(Func *f, Statement *s) {
    if (!f->statements) f->statements = listCreate();
    return listAppend(f->statements, s);
}

/* Private procedures
 * This procedure always be called by function call expression
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
        /* Every test case should have a bool return value to indicate whether
        * the test case is running success */
        abortWithMsg("Can't found return in a test case");
    }
}
