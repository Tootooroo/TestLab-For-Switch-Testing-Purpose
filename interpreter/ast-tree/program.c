/* program.c */

#include "program.h"
#include "wrapper.h"
#include "statement.h"

/* Private prototype */
private void programCompute(Program *p);

Program * programGenerate(list *stmts) {
    Program *p = (Program *)zMalloc(sizeof(Program));

    PROGRAM_SET_STMTS(p, stmts);
    PROGRAM_SET_COMPUTE_ROUTINE(p, programCompute);

    return p;
}

/* Private procedures */
private void programCompute(Program *p) {
    Scope *s = scopeGenerate();

    statementCompute_untilReturn(PROGRAM_GET_STMTS(p), s);
}
