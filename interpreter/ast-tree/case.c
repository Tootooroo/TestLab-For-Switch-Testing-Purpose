/* case.c */

#include "case.h"

_Status_t caseAppendStatement(Case *c, Statement *s) {
    /* Initialize statements list */
    if (CASE_IS_EMPTY_CASE(c)) c->statements = listCreate();

    return OK;
}
