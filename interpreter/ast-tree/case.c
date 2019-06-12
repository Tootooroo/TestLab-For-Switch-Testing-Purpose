/* case.c */

#include "wrapper.h"
#include "case.h"

#define CASE_SET_OMPUTE_ROUTINE(C, COMPUTE) ((C)->compute = COMPUTE)

/* Private prototypes */
_Bool __caseComputing(Case *c);

/* Public procedures */
Case * caseGenerate() {
    Case *c = (Case *)zMalloc(sizeof(Case));
    CASE_SET_OMPUTE_ROUTINE(c, __caseComputing);
    return c;
}

_Status_t caseAppendStatement(Case *c, Statement *s) {
    /* Initialize statements list */
    if (CASE_IS_EMPTY_CASE(c)) c->statements = listCreate();

    listAppend(c->statements, s);
    return OK;
}

/* Private procedures */
_Bool __caseComputing(Case *c) {
    if (CASE_IS_EMPTY_CASE(c)) return false;

    listNode *current;
    listIter iter = listGetIter(c->statements, LITER_FORWARD);

    while (current = listNext(&iter)) {
        Statement *s = current->value;
        StatementTrack st = s->compute(s);

        // To check that is a return statement being computed.
        if (st.id == RETURN_STATEMENT_ID) {
            _Bool *returnVal = (_Bool *)st.v;
            if (*returnVal == true) {
                free(returnVal);
                return true;
            }

            free(returnVal);
            return false;
        }
    }

    /* Every test case should have a bool return value to indicate whether
     * the test case is running success */
    abortWithMsg("Can't found return in a test case");
}
