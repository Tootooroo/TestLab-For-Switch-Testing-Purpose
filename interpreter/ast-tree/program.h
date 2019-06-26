/* program.h */

#ifndef _AST_TREE_PROGRAM_H_
#define _AST_TREE_PROGRAM_H_

#include "list.h"
#include "case.h"
#include "scope.h"

typedef struct Program {
    list *statements;
    void (*compute)(struct Program *);
} Program;

/* Member function implemented as macros */
#define PROGRAM_SET_STMTS(P, STMTS) ((P)->statements = (STMTS))
#define PROGRAM_GET_STMTS(P) ((P)->statements)

#define PROGRAM_SET_COMPUTE_ROUTINE(P, COMPUTE) ((P)->compute = (COMPUTE))
#define PROGRAM_COMPUTING(P) ((P)->compute((P)))

/* Prototypes */
Program * programGenerate(list *statements);

#endif /* _AST_TREE_PROGRAM_H_ */
