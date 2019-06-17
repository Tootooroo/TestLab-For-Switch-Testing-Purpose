/* program .c */

#include "program.h"
#include "wrapper.h"

Program * programGenerate(Func *entrance) {
    Program *p = (Program *)zMalloc(sizeof(Program));

    PROGRAM_SET_ENTRANCE(p, entrance);
    PROGRAM_SET_COMPUTE_ROUTINE(p, computeRtn);

    return p;
}

void programCompute(Program *p, Scope *s) {
    Func *start_ = p->start_;

    start_->compute(start_, s);
}
