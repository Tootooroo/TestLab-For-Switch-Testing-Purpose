/* statement.h */

#ifndef _AST_TREE_STATEMENT_H_
#define _AST_TREE_STATEMENT_H_

#include "type.h"
#include "expression.h"
#include "list.h"
#include "scope.h"

#include "variable.h"

typedef enum {
    IF_STATEMENT_ID,
    RETURN_STATEMENT_ID,
    VAR_DECL_STATEMENT_ID,
    FUNC_DECL_STATEMENT_ID,
    EXPR_STATEMENT_ID,
    ASSIGN_STATEMENT_ID
} StatementID;

typedef struct StatementTrack {
    /* pointer reference to the statement which
     * return this track */
    void *s;
    StatementID id;
    /* field v is used to hold value
     * return by a return statement */
    Variable v;
} StatementTrack;

/* Base statement structure */
typedef struct Statement {
    StatementTrack (*compute)(struct Statement *, Scope *);
} Statement;

typedef struct IfStatement {
    Statement base;
    Expression *conditionExpr;
    list *trueStatements;
    list *falseStatements;
} IfStatement;

typedef struct ReturnStatement {
    Statement base;
    Expression *expr;
} ReturnStatement;

typedef struct VarDeclStatement {
    Statement base;
    /* list of pair(identifier, expr) */
    list *varDeclExprs;
} VarDeclStatement;

typedef struct FuncDeclStatement {} FuncDeclStatement;

typedef struct AssignmentStatement {
    /* compute in AssignmentStatement will compute expr in
     * assigns then generate some variables */
    Statement base;
    /* list of pair(identifier, expr) */
    list *assigns;
} AssignmentStatement;

typedef struct ImportStatement {
    Statement base;
    /* list<char *> */
    list *importSymbols;
    /* Name of the module be imported */
    char *from;
} ImportStatement;

typedef struct ExpressionStatement {

} ExpressionStatement;

typedef struct ObjectDeclStatement {
    Statement base;
    char *objectType;
    char *parent;
    /* list of pair(parent's member, value) */
    list *overWrites;
    /* list of pair(member name, type name) */
    list *members;
} ObjectDeclStatement;

typedef struct ObjectDeclBody {
    list *newMembers;
    list *overWrites;
} ObjectDeclBody;

typedef struct ObjectDeclItem {
    /* 0: Overwrite
     * 1: member */
    int type;
    pair *item;
} ObjectDeclItem;

/* Type alias */
typedef StatementTrack (*stmtCompute)(struct Statement *, Scope *);

/* Member function implement as macros */
#define STATEMENT_COMPUTE(S, SCOPE) ((S)->compute((S), (SCOPE))

/* Prototypes */

/* Computing a list of statments until a return statement is computed. */
StatementTrack statementCompute_untilReturn(list *listOfStmt, Scope *);

Statement statementGenerate(stmtCompute compute);

/* If statement */
/* Parameters:
 * compute_ - Procedure to compute if statement.
 * expr_ - condition expression
 * true_stmts - Will be computed if expr_ is true
 * false_stmts - Will be computed if expr_ is false */
IfStatement * ifStatementGenerate(Expression expr_, list *true_stmts, list *false_stmts);

/* Variable statement */
/* Parameters:
 * expr_ - list of expressions in a variable declaration statement */
VarDeclStatement * varDeclStmtGenerate(stmtCompute compute_, list *expr_);

/* Object declaration statement */
ObjectDeclStatement * objDeclStmtDefault();
ObjectDeclStatement * objDeclStmtGen(char *objType, list *members, char *parent, list *overWrites);
ObjectDeclBody * objBodyGen();
ObjectDeclItem * objItemGen();

/* Import statement */
ImportStatement * importStmtDefault();
ImportStatement * importStmtGen(list *symbols, char *from);

/* Return statement */
ReturnStatement * returnStmtDefault();
ReturnStatement * returnStmtGen(Expression *expr);

#endif /* _AST_TREE_STATEMENT_H_ */
