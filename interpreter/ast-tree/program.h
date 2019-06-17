/* program.h */

#ifndef _AST_TREE_PROGRAM_H_
#define _AST_TREE_PROGRAM_H_

#include "list.h"
#include "case.h"
#include "scope.h"

typedef struct Program {
    /* Test case "start_" is the
     * entrance of a program */
    Func *start_;
    /* This procedure will compute test cjase
     * which named "start_" */
    void (*compute)(struct Program *, Scope *);
} Program;

/* Member function implemented as macros */
#define PROGRAM_SET_ENTRANCE(P, EN) ((P)->start_ = (EN))
#define PROGRAM_GET_ENTRANCE(P) ((P)->start_)

#define PROGRAM_SET_COMPUTE_ROUTINE(P, COMPUTE) ((P)->compute = (COMPUTE))
#define PROGRAM_COMPUTING(P) ((P)->compute((P)))

/* Prototypes */
Program * programGenerate(Func *entrance);


#endif /* _AST_TREE_PROGRAM_H_ */
