/* Case.h */

#ifndef _AST_TREE_CASE_H_
#define _AST_TREE_CASE_H_

#include "list.h"
#include "type.h"
#include "statement.h"

typedef struct Case {
    /* Statements within the test case */
    list *statements;
    /* Note: Every test case will return true or false */
    _Bool (*compute)(struct Case *);
} Case;

/* Member function implemented as macros */
#define CASE_IS_EMPTY_CASE(C) ((C)->statements == null)
#define CASE_SET_STATEMENT_LIST(C, L) ((C)->statements = (L))
#define CASE_COMPUTING(C) ((C)->compute(C))

/* Prototypes */
Case * caseGenerate();
_Status_t caseAppendStatement(Case *c, Statement *s);

#endif /* _AST_TREE_CASE_H_ */
