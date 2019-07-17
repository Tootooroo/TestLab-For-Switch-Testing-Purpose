/* Statement.c */

#include "statement.h"
#include "wrapper.h"
#include "primitive.h"
#include "string.h"
#include "scope.h"
#include "pair.h"
#include "list.h"
#include "expression.h"

/* Private Prototypes */
private StatementTrack ifStatement_Compute(Statement *stmt, Scope *scope);
private StatementTrack varDeclStmtCompute(Statement *stmt, Scope *scope);
private StatementTrack objStmtCompute(Statement *stmt, Scope *scope);
private StatementTrack importStmtCompute(Statement *stmt, Scope *scope);
private StatementTrack returnStmtCompute(Statement *stmt, Scope *scope);
private StatementTrack funcDeclStmtCompute(Statement *stmt, Scope *scope);
private StatementTrack exprStmtCompute(Statement *stmt, Scope *scope);

#define STrack_Default() { .error = STMT_ERROR_NONE, .s = null, .id = 0, .v = null }
#define STrack_GEN(STMT_REF, STMT_TYPE, STMT_RET)\
    { .error = STMT_ERROR_NONE, .s = (STMT_REF), .id = (STMT_TYPE), .v = (STMT_RET) }

/* Public Procedures */

// Base statement
Statement statementGenerate(stmtCompute compute) {
    Statement stmt = { .compute = compute };

    return stmt;
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
IfStatement * ifStatementGenerate(Expression *expr, list *true_stmts, list *false_stmts) {

    IfStatement *ifStmt = (IfStatement *)zMalloc(sizeof(IfStatement));
    ifStmt->base = statementGenerate(ifStatement_Compute);
    ifStmt->conditionExpr = expr;
    ifStmt->trueStatements = true_stmts;
    ifStmt->falseStatements = false_stmts;

    return ifStmt;
}

// Variable declaration statement
VarDeclStatement * varDeclStmtGenerate(char *type) {
    VarDeclStatement *varStmt = (VarDeclStatement *)zMalloc(sizeof(VarDeclStatement));
    varStmt->base = statementGenerate(varDeclStmtCompute);
    varStmt->type = type;
    varStmt->varDeclExprs = null;

    return varStmt;
}

/* pair<identifier, assignmentExpr> */
_Status_t varDeclAddExpr(VarDeclStatement *stmt, Expression *expr) {
    if (!stmt->varDeclExprs) stmt->varDeclExprs = listCreate();

    char *ident;
    if (exprType(expr) == EXPR_TYPE_ASSIGN) {
        AssignmentExpression *aExpr = (AssignmentExpression *)expr;
        IdentExpression *identExpr = (IdentExpression *)aExpr->l;
        ident = IDENT_EXPR_GET(identExpr);
    } else if (exprType(expr) == EXPR_TYPE_IDENTIFIER) {
        IdentExpression *iExpr = (IdentExpression *)expr;
        ident = IDENT_EXPR_GET(iExpr);
    } else {
        return ERROR;
    }

    pair *p = pairGen(strdup(ident), expr);
    return listAppend(stmt->varDeclExprs, p);
}

// Object declaration statement */
ObjectDeclStatement * objDeclStmtDefault() {
    ObjectDeclStatement *obj = (ObjectDeclStatement *)zMalloc(sizeof(ObjectDeclStatement));
    obj->base = statementGenerate(objStmtCompute);
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
    ObjectDeclItem *item = (ObjectDeclItem *)zMalloc(sizeof(ObjectDeclBody));
    return item;
}

_Status_t objAddMember(ObjectDeclStatement *obj, pair *member) {
    list *members = obj->members;

    return listAppend(members, member);
}

_Status_t objAddOverWrite(ObjectDeclStatement *obj, pair *overWrite) {
    list *overWrites = obj->overWrites;

    return listAppend(overWrites, overWrite);
}

// Import statement
ImportStatement * importStmtDefault() {
    ImportStatement *iStmt = (ImportStatement *)zMalloc(sizeof(ImportStatement));
    iStmt->base = statementGenerate(importStmtCompute);
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
    rStmt->base = statementGenerate(returnStmtCompute);

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
private StatementTrack ifStatement_Compute(Statement *stmt, Scope *scope) {
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

private StatementTrack varDeclStmtCompute(Statement *stmt, Scope *scope) {
    StatementTrack st = STrack_GEN(stmt, DECL_STATEMENT_ID, NULL);

    int primitiveType = false;
    VarDeclStatement *varDeclStmt = (VarDeclStatement *)stmt;

    primitiveType = isPrimitiveType(VAR_DECL_STMT_TYPE(varDeclStmt));

    list *declPairs = varDeclStmt->varDeclExprs;
    listIter iter = listGetIter(declPairs, LITER_FORWARD);

    listNode *current;
    pair *currentPair;

    // First place identifier into scope.
    while ((current = listNext(&iter)) != null) {
        currentPair = current->value;

        char *varIdent = PAIR_GET_LEFT(currentPair);
        if (primitiveType != -1) {
            /* Primitive */
            Variable *v = null;

            switch (primitiveType) {
                case PRIMITIVE_TYPE_INT:
                    v = varGen(strdup(varIdent), VAR_PRIMITIVE_INT,
                            primitiveDefault(PRIMITIVE_TYPE_INT));
                    break;
                case PRIMITIVE_TYPE_STR:
                    v = varGen(strdup(varIdent), VAR_PRIMITIVE_STR,
                            primitiveDefault(PRIMITIVE_TYPE_STR));
                    break;
            }

            scopeNewPrimitive(scope, pairGen(strdup(varIdent), v));
        } else {
            /* Object */
            Template *t = scopeGetTemplate(scope, VAR_DECL_STMT_TYPE(varDeclStmt));
            Variable *var_o = varGen(strdup(varIdent), VAR_OBJECT, template2Object(t));

            scopeNewObject(scope, pairGen(strdup(varIdent), var_o));
        }
    }

    listRewind(&iter);

    // Compute expressions within decl statement.
    while ((current = listNext(&iter)) != null) {
        /* Only assignment can be reside in decl statement
         * otherwise error occur. */
        currentPair = current->value;

        // Is assignment ?
        Expression *expr = PAIR_GET_RIGHT(currentPair);
        if (exprType(expr) != EXPR_TYPE_ASSIGN) {
            st.error = STMT_ERROR_ABORT;
            return st;
        }

        /* Compute the assignment and after computation finish
         * an initial value will be place into scope. */
        expr->compute(expr, scope);
    }

    return st;

}

// Create a object template and store into Scope
private StatementTrack objStmtCompute(Statement *stmt, Scope *scope) {
    StatementTrack st = STrack_GEN(stmt, DECL_STATEMENT_ID, null);

    ObjectDeclStatement *oStmt = (ObjectDeclStatement *)stmt;

    Template *new = templateGen(strdup(oStmt->objectType));

    // Is parent exists ?
    if (oStmt->parent) {
        Template *t = scopeGetTemplate(scope, oStmt->parent);
        if (isNull(t)) {
            ST_SET_ERROR(&st, STMT_ERROR_ABORT);
            return st;
        }

        if (t->members) {
            /* Copy members from parent template */
            new->members = listDup(t->members);
            /* Initialize members inherit from parent */
            if (oStmt->overWrites) {
                listIter iter = listGetIter(oStmt->overWrites, LITER_FORWARD);

                pair *overW_pair;
                list *members = new->members;
                Variable *var;

                while ((overW_pair = (pair *)listNext_v(&iter))) {
                    var = (Variable *)listSearch(members, PAIR_GET_LEFT(overW_pair));
                    varAssign_(var, PAIR_GET_RIGHT(overW_pair));
                }
            }
        }
    }

    if (oStmt->members) templateAddMembers(new, oStmt->members);

    /* Store into current scope */
    scopeNewTemplate(scope, pairGen(strdup(TEMPLATE_TYPE(new)), new));

    return st;
}

private StatementTrack importStmtCompute(Statement *stmt, Scope *scope) {}

private StatementTrack returnStmtCompute(Statement *stmt, Scope *scope) {
    StatementTrack st = STrack_Default();
    ReturnStatement *rStmt = (ReturnStatement *)stmt;

    Expression *expr = rStmt->expr;

    ST_SET_RET_VAR(&st, expr->compute(expr, scope));
    ST_SET_REL_STMT(&st, stmt);
    ST_SET_STMT_TYPE(&st, RETURN_STATEMENT_ID);

    return st;
}

private StatementTrack funcDeclStmtCompute(Statement *stmt, Scope *scope) {
    StatementTrack st = STrack_Default();
    FuncDeclStatement *fStmt = (FuncDeclStatement *)stmt;

    Func *f = fStmt->f;

    scopeNewFunc(scope, pairGen(strdup(f->identifier), f));

    ST_SET_REL_STMT(&st, stmt);
    ST_SET_STMT_TYPE(&st, FUNC_DECL_STATEMENT_ID);

    return st;
}

private StatementTrack exprStmtCompute(Statement *stmt, Scope *scope) {
    StatementTrack st = STrack_Default();

    ST_SET_REL_STMT(&st, stmt);
    ST_SET_STMT_TYPE(&st, EXPR_STATEMENT_ID);

    Expression *expr = ((ExpressionStatement *)stmt)->expr;

    ST_SET_RET_VAR(&st, expr->compute(expr, scope));

    if (!ST_RET_VAR(&st))
        ST_SET_ERROR(&st, STMT_ERROR_ABORT);

    return st;
}

#ifdef _AST_TREE_TESTING_

#include "test.h"
#include "expression.h"

void stmtTest(void **state) {
    funcDeclStmtTest();
    exprStmtTest();
    varDeclStmtTest();
}

void objDeclStmtTest(void) {
    Scope *scope = scopeGenerate();

    // Object declaration
    ObjectDeclStatement *objStmt = objDeclStmtGen(strdup("AA"), NULL, NULL, NULL);

    int a = 1;
    int b = 2;

    objAddMember(objStmt, pairGen(strdup("a"), varGen(strdup("a"), VAR_PRIMITIVE_INT, &a)));
    objAddMember(objStmt, pairGen(strdup("b"), varGen(strdup("b"), VAR_PRIMITIVE_INT, &b)));

    Statement *base = (Statement *)objStmt;

    base->compute(base, scope);

    // Declare an variable with object type.
}

void varDeclStmtTest(void) {
    Scope *scope = scopeGenerate();

    VarDeclStatement *varStmt = varDeclStmtGenerate(strdup("Int"));

    int a = 1;
    Expression *assignExpr = assignExprGen(identExprGen(strdup("a")), constExprGen(&a, PRIMITIVE_TYPE_INT));
    varDeclAddExpr(varStmt, assignExpr);

    Statement *baseStmt = (Statement *)varStmt;

    baseStmt->compute(baseStmt, scope);

    Expression *iExpr = identExprGen(strdup("a"));
    Variable *v = iExpr->compute(iExpr, scope);

    assert_non_null(v);
    assert_int_equal(getPrimitive_int(v->p), 1);
}

void funcDeclStmtTest(void) {
    Scope *scope = scopeGenerate();

    FuncDeclStatement *fStmt = funcDeclStmtDefault();

    Func *f = funcGen(strdup("f"), "Int", NULL, scopeGenerate());
    funcAddParam(f, paramGen(strdup("a"), strdup("Int")));

    int b = 1;
    Statement *stmt = (ReturnStatement *)returnStmtGen(plusExprGen(identExprGen(strdup("a")),
                                                                   constExprGen(&b, PRIMITIVE_TYPE_INT)));
    funcAppendStatements(f, stmt);

    FUNC_DECL_STMT_SET_FUNC(fStmt, f);

    Statement *baseStmt = (Statement *)fStmt;
    baseStmt->compute(baseStmt, scope);

    // Try to call expression we just define.
    FuncCallExpression *fExpr = (FuncCallExpression *)funcCallExprGen(strdup("f"), NULL);

    int c = 2;
    funcCallAddArg(fExpr, arguGen(constExprGen(&c, PRIMITIVE_TYPE_INT)));

    Expression *baseExpr = (Expression *)fExpr;

    Variable *v = baseExpr->compute(baseExpr, scope);

    assert_non_null(v);
    assert_int_equal(getPrimitive_int(v->p), 3);
}

// fixme: Do expression statement test
void exprStmtTest(void) {}

#endif /* _AST_TREE_TESTING_ */
