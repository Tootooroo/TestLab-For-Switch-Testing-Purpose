/* Statement.c */

#include "statement.h"
#include "wrapper.h"

/* Private Prototypes */
Private StatementTrack ifStatement_Compute(Statement *stmt, Scope *scope);

/* Public Procedures */
Statement statementGenerate(stmtCompoute compute_) {
    Statement s = {
        .compute = compute_;
    };
    return s;
}

IfStatement * ifStatementGenerate(Expression expr_, list *true_stmts, list *false_stmts) {

    IfStatement *ifStmt = (IfStatement *)zMalloc(sizeof(IfStatement));
    ifStmt->base = statementGenerate(ifStatement_Compute);
    ifStmt->conditionExpr = expr_;
    ifStmt->trueStatements = true_stmts;
    ifStmt->falseStatements = false_stmts;

    return ifStmt;
}

VarDeclStatement * varDeclStmtGenerate(stmtCompute compute_, list *expr_) {
    VarDeclStatement *varStmt = (VarDeclStatement *)zMalloc(sizeof(VarDeclStatement));
    varStmt->base = statementGenerate(compute_);
    varStmt->varDeclExprs = expr_;

    return varStmt;
}

StatementTrack varDeclStmt_compute(Statement *stmt, Scope *scope) {

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
