/* program.c */

#include "program.h"
#include "wrapper.h"

Program * programGenerate(Statement *stmts) {
    Program *p = (Program *)zMalloc(sizeof(Program));

    PROGRAM_SET_STMTS(p, stmts);
    PROGRAM_SET_COMPUTE_ROUTINE(p, computeRtn);

    return p;
}

void programCompute(Program *p) {
    Scope *s = scopeGenerate();
    statementCompute_untilReturn(p->state, s);
}
