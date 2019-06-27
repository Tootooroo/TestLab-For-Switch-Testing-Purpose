/* Statement.c */

#include "statement.h"
#include "wrapper.h"

/* Private Prototypes */
Private StatementTrack ifStatement_Compute(Statement *stmt, Scope *scope);
Private StatementTrack varDeclStmtCompute(Statement *stmt, Scope *scope);
Private StatementTrack objStmtCompute(Statement *stmt, Scope *scope);
Private StatementTrack importStmtCompute(Statement *stmt, Scope *scope);
Private StatementTrack returnStmtCompute(Statement *stmt, Scope *scope);
Private StatementTrack funcDeclStmtCompute(Statement *stmt, Scope *scope);
Private StatementTrack exprStmtCompute(Statement *stmt, Scope *scope);

/* Public Procedures */

// Base statement
Statement statementGenerate(stmtCompoute compute_) {
    Statement s = {
        .compute = compute_;
    };
    return s;
}

StatementTrack statementCompute_untilReturn(list *listOfStmt, Scope *scope) {
    listNode *current;
    listIter iter = listGetIter(listOfStmt, LITER_FORWARD);

    StatementTrack st;
    Statement *stmt;
    while ((current = listNext(&iter)) != null) {
        stmt = (Statement *)current->value;

        st = STATEMENT_COMPUTE(stmt, scope);
        if (st.id == RETURN_STATEMENT_ID) {
            return st;
        }
    }

    return st;
}

// If statement
IfStatement * ifStatementGenerate(Expression expr_, list *true_stmts, list *false_stmts) {

    IfStatement *ifStmt = (IfStatement *)zMalloc(sizeof(IfStatement));
    ifStmt->base = statementGenerate(ifStatement_Compute);
    ifStmt->conditionExpr = expr_;
    ifStmt->trueStatements = true_stmts;
    ifStmt->falseStatements = false_stmts;

    return ifStmt;
}

// Variable declaration statement
VarDeclStatement * varDeclStmtGenerate(list *expr_) {
    VarDeclStatement *varStmt = (VarDeclStatement *)zMalloc(sizeof(VarDeclStatement));
    varStmt->base = statementGenerate(varDeclStmtCompute);
    varStmt->varDeclExprs = expr_;

    return varStmt;
}

// Object declaration statement */
ObjectDeclStatement * objDeclStmtDefault() {
    ObjectDeclStatement *obj = (ObjectDeclStatement *)zMalloc(sizeof(ObjectDeclStatement));
    obj->base.compute = statementGenerate(objStmtCompute);
    return obj;
}

ObjectDeclStatement * objDeclStmtGen(char *objType, list *members, char *parent, list *overWrites) {
    ObjectDeclStatement *obj = objDeclStmtDefault();
    obj->objectType = objType;
    obj->members = members;
    obj->parent = parent;
    obj->overWrites = overWrites;

    return obj;
}

ObjectDeclBody * objBodyGen() {
    ObjectDeclBody *body = (ObjectDeclBody *)zMalloc(sizeof(ObjectDeclBody));
    return body;
}

ObjectDeclItem * objItemGen() {
    ObjectDeclItem *item = (ObjectDeclBody *)zMalloc(sizeof(ObjectDeclBody));
    return item;
}

// Import statement
ImportStatement * importStmtDefault() {
    ImportStatement *iStmt = (ImportStatement *)zMalloc(sizeof(ImportStatement));
    iStmt->base.compute = statementGenerate(importStmtCompute);
    return iStmt;
}

ImportStatement * importStmtGen(list *symbols, char *from) {
    ImportStatement *iStmt = importStmtDefault();
    iStmt->importSymbols = symbols;
    iStmt->from = from;

    return iStmt;
}

// Return statement
ReturnStatement * returnStmtDefault() {
    ReturnStatement *rStmt = (ReturnStatement *)zMalloc(sizeof(ReturnStatement));
    rStmt->base.compute = statementGenerate(returnStmtCompute);

    return rStmt;
}
ReturnStatement * returnStmtGen(Expression *expr) {
    ReturnStatement *rStmt = returnStmtDefault();
    rStmt->expr = expr;

    return rStmt;
}

// Function Declaration statement
FuncDeclStatement * funcDeclStmtDefault() {
    FuncDeclStatement *fStmt = (FuncDeclStatement *)zMalloc(sizeof(FuncDeclStatement));
    fStmt->base.compute = funcDeclStmtCompute;

    return fStmt;
}

FuncDeclStatement * funcDeclStmtGen(Func *f) {
    FuncDeclStatement *fStmt =funcDeclStmtDefault();
    fStmt->f = f;

    return fStmt;
}

// Expression Statement
ExpressionStatement * exprStmtDefault() {
    ExpressionStatement * eStmt = (ExpressionStatement *)zMalloc(sizeof(ExpressionStatement));
    eStmt->base.compute = exprStmtCompute;
    return eStmt;
}

ExpressionStatement * exprStmtGen(Expression *expr) {
    ExpressionStatement *eStmt = exprStmtDefault();
    EXPR_STMT_SET_EXPR(eStmt, expr);

    return eStmt;
}

/* Private Procedures */
Private StatementTrack ifStatement_Compute(Statement *stmt, Scope *scope) {
    StatementTrack st = { .s = stmt, .id = IF_STATEMENT_ID };

    list *beComputed;
    IfStatement *if_stmt = (IfStatement *)stmt;
    Expression *condExpr = if_stmt->conditionExpr;

    if (condExpr->compute(condExpr, scope))
        beComputed = if_stmt->trueStatements;
    else
        beComputed = if_stmt->falseStatements;

    st = statementCompute_untilReturn(beComputed, scope);

    return st;
}

Private StatementTrack varDeclStmtCompute(Statement *stmt, Scope *scope) {

}

Private StatementTrack objStmtCompute(Statement *stmt, Scope *scope) {

}

Private StatementTrack importStmtCompute(Statement *stmt, Scope *scope) {

}

Private StatementTrack returnStmtCompute(Statement *stmt, Scope *scope) {

}

Private StatementTrack funcDeclStmtCompute(Statement *stmt, Scope *scope) {

}

Private StatementTrack exprStmtCompute(Statement *stmt, Scope *scope) {

}
