/* program .c */

#include "program.h"
#include "wrapper.h"

Program * programGenerate(Case *entrance, void (*computeRtn)(struct Program *)) {
    Program *p = (Program *)zMalloc(sizeof(Program));

    PROGRAM_SET_ENTRANCE(p, entrance);
    PROGRAM_SET_COMPUTE_ROUTINE(p, computeRtn);

    return p;
}
