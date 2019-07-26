/* program.c */

#include "program.h"
#include "wrapper.h"
#include "statement.h"

#include "module.h"

/* Private variables */
Scope *global_scope;

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
    global_scope = scopeGenerate();

    /* The scope generate just now is in the top
     * of scope layer which means it's global scope. */
    global_scope->isGlobal = true;

    /* Point mTbl to module table */
    global_scope->mTbl = &moduleTable;

    statementCompute_untilReturn(PROGRAM_GET_STMTS(p), global_scope);
}
