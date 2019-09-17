/* statement.h */
#ifndef _AST_TREE_STATEMENT_H_
#define _AST_TREE_STATEMENT_H_

#include "type.h"
#include "expression.h"
#include "list.h"
#include "scope.h"
#include "func.h"

#include "variable.h"

typedef enum {
    STMT_ERROR_NONE,
    STMT_ERROR_WARN,
    STMT_ERROR_ABORT
} stmtErrorType;

typedef enum {
    IF_STATEMENT_ID,
    RETURN_STATEMENT_ID,
    DECL_STATEMENT_ID,
    FUNC_DECL_STATEMENT_ID,
    EXPR_STATEMENT_ID,
    ASSIGN_STATEMENT_ID
} StatementID;

typedef struct StatementTrack {
    /* Error indicator */
    stmtErrorType error;
    /* pointer reference to the statement which
     * return this track */
    void *s;
    StatementID id;
    /* field v is used to hold value
     * return by a return statement */
    Variable *v;
} StatementTrack;

#define ST_SET_REL_STMT(ST, STMT) (((ST)->s = (STMT)))
#define ST_REL_STMT(ST) (((ST)->s))

#define ST_SET_STMT_TYPE(ST, T) ((ST)->id = (T))
#define ST_STMT_TYPE(ST) ((ST)->id)

#define ST_SET_ERROR(ST, E) ((ST)->error = (E))
#define ST_ERROR_TYPE(ST) ((ST)->error)

#define ST_SET_RET_VAR(ST, V) ((ST)->v = (V))
#define ST_RET_VAR(ST) ((ST)->v)

/* Base statement structure */
typedef struct Statement {
    StatementID type;
    StatementTrack (*compute)(struct Statement *, Scope *);
} Statement;

/* Type alias */
typedef StatementTrack (*stmtCompute)(struct Statement *, Scope *);

/* Member function implement as macros */
#define STATEMENT_2_BASE(S) ((Statement *)(S))
/* StatementTrack (*)(Statement *, Scope *) */
#define STATEMENT_COMPUTE(S, SCOPE) (STATEMENT_2_BASE(S)->compute(STATEMENT_2_BASE(S), (SCOPE)))
/* StatementID (*)(Statement *) */
#define STATEMENT_TYPE(S) ({ STATEMENT_2_BASE(S)->type; })
/* void (*)(Statement *, StatementID) */
#define STATEMENT_SET_TYPE(S, T) (STATEMENT_2_BASE(S)->type = (T))

/* Prototypes */
Statement statementGenerate(stmtCompute compute);
/* Computing a list of statments until a return statement is computed. */
StatementTrack statementCompute_untilReturn(list *listOfStmt, Scope *);

/* If statements */
typedef struct IfStatement {
    Statement base;
    Expression *conditionExpr;
    list *trueStatements;
    list *falseStatements;
} IfStatement;

/* Expression * (*)(IfStatement *, Expression *) */
#define IF_STMT_SET_COND(S, COND_EXPR) ((S)->conditionExpr = (COND_EXPR))
/* list * (*)(IfStatement *, list *) */
#define IF_STMT_SET_TRUE_STMTS(S, STMTS) ((S)->trueStatements = (STMTS))
/* list * (*)(IfStatement *, list *) */
#define IF_STMT_SET_FALSE_STMTS(S, STMTS) ((S)->falseStatements = (STMTS))

/* Parameters:
 * compute_ - Procedure to compute if statement.
 * expr_ - condition expression
 * true_stmts - Will be computed if expr is true
 * false_stmts - Will be computed if expr is false */
IfStatement * ifStatementGenerate(Expression *expr, list *true_stmts, list *false_stmts);

/* Return statement */
typedef struct ReturnStatement {
    Statement base;
    Expression *expr;
} ReturnStatement;

/* Expression * (*)(ReturnStatement *, Expression *) */
#define RET_STMT_SET_EXPR(S, E) ((S)->expr = (E))

ReturnStatement * returnStmtDefault();
ReturnStatement * returnStmtGen(Expression *expr);

/* Variable declaration statement */
typedef enum {
    BASIC_TYPE = 0,
    ARRAY_TYPE = 1,
    MAP_TYPE = 2
} TYPE_QUALIFIER;

typedef struct typeInfo {
    char *type;
    TYPE_QUALIFIER q;
} typeInfo;

typedef struct VarDeclStatement {
    Statement base;
    typeInfo *type;
    /* list of pair(identifier, assignmentExpr) */
    list *varDeclExprs;
} VarDeclStatement;

/* char * (*)(typeInfo *) */
#define TYPE_INFO_TYPE(T) ((T)->type)
/* int (*)(typeInfo *) */
#define TYPE_INFO_QUALIFIER(T) ((T)->q)
/* char * (*)(VarDeclStatement *) */
#define VAR_DECL_STMT_TYPE(V_D_STMT) ((V_D_STMT)->type)
/* char * (*)(VarDeclStatement *, char *) */
#define VAR_DECL_STMT_SET_TYPE(V_D_STMT, T) ((V_D_STMT)->type = T)
/* list * (*)(VarDeclStatement *, list *) */
#define VAR_DECL_STMT_SET_EXPRS(V_D_STMT, EXPRS) ((V_D_STMT)->varDeclExprs = (EXPRS))

/* Parameters:
 * expr_ - list of expressions in a variable declaration statement */
VarDeclStatement * varDeclStmtGenerate(typeInfo *type);
_Status_t varDeclAddExpr(VarDeclStatement *, Expression *);
typeInfo * buildTypeInfo(char *type, int qual);

/* Function declaration statement */
typedef struct FuncDeclStatement {
    Statement base;
    Func *f;
} FuncDeclStatement;

/* Func * (*)(FuncDeclStatement *, Func *) */
#define FUNC_DECL_STMT_SET_FUNC(S, F) ((S)->f = (F))

FuncDeclStatement * funcDeclStmtDefault();
FuncDeclStatement * funcDeclStmtGen(Func *);

/* Import statement */
typedef struct ImportStatement {
    Statement base;
    /* list<char *> */
    list *importSymbols;
    /* Name of the module be imported */
    char *from;
} ImportStatement;

ImportStatement * importStmtDefault();
ImportStatement * importStmtGen(list *symbols, char *from);

/* Expression statement */
typedef struct ExpressionStatement {
    Statement base;
    Expression *expr;
} ExpressionStatement;

/* Expression * (*)(ExpressionStatement *, Expression *) */
#define EXPR_STMT_SET_EXPR(ES, E) ((ES)->expr = (E))

ExpressionStatement * exprStmtDefault();
ExpressionStatement * exprStmtGen(Expression *expr);

/* Object declaration statement */
typedef struct ObjectDeclStatement {
    Statement base;
    char *objectType;
    char *parent;
    /* list of Variable */
    list *overWrites;
    /* list of Variable */
    list *members;
} ObjectDeclStatement;

typedef struct ObjectDeclBody {
    /* list<Variable> */
    list *newMembers;
    list *overWrites;
} ObjectDeclBody;

typedef enum { OBJECT_OVER_WRITE, OBJECT_MEMBER } OBJ_ITEM_TYPE;

typedef struct ObjectDeclItem {
    /* 0: Overwrite
     * 1: member */
    OBJ_ITEM_TYPE type;
    /* Variable */
    Variable *item;
} ObjectDeclItem;

ObjectDeclStatement * objDeclStmtDefault();
ObjectDeclStatement * objDeclStmtGen(char *objType, list *members, char *parent, list *overWrites);
ObjectDeclBody * objBodyGen();
ObjectDeclItem * objItemGen();
_Status_t objAddMember(ObjectDeclStatement *obj, pair *member);
_Status_t objAddOverWrite(ObjectDeclStatement *obj, pair *overWrite);

#ifdef _AST_TREE_TESTING_

void stmtTest(void **state);

#endif /* _AST_TREE_TESTING_ */

#endif /* _AST_TREE_STATEMENT_H_ */
