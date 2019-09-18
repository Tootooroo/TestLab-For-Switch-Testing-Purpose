%union {
    char *str;
    int integer;
    list *list_;
    Program *program;
    Statement *statement;
    Expression *expression;

    pair *Pair;
    typeInfo *type_info;

    ObjectDeclBody *objDeclBody;
    ObjectDeclItem *objDeclItem;
}

%token IMPORT FROM
%token RETURN
%token OBJECT
%token BELONG

%token MACHINE_OPERATIONS

%token IF ELSE

%token ARRAY_MARK MAP_MARK
%token COMMA COLON SEMICOLON
%token OPEN_CURVE_BRACKET CLOSE_CURVE_BRACKET

%token INT
%token STR

%token <str> IDENTIFIER
%token <str> NUM
%token <str> STR_LITERAL


%left PERCENTAGE
%left EQUAL LESS_THAN GREATER_THAN LESS_OR_EQUAL GREATER_OR_EQUAL NOT_EQUAL
%left PLUS MINUS
%left MUL DIV
%right ASSIGNMENT
%left OPEN_PAREN CLOSE_PAREN
%left DOT OPEN_SQUARE_BRACKET CLOSE_SQUARE_BRACKET

/* Statements */
%type   <program>       PROGRAM
%type   <list_>         STATEMENTS
%type   <statement>     STATEMENT
%type   <statement>     IF_STATEMENT
%type   <statement>     OBJECT_DECL_STATEMENT
%type   <statement>     IMPORT_STATEMENT
%type   <statement>     RETURN_STATEMENT
%type   <statement>     VAR_DECL_STATEMENT
%type   <statement>     FUNC_DECL_STATEMENT
%type   <statement>     EXPRESSION_STATEMENT
%type   <statement>     IF_STATEMENT_WITH_ELSE

%type   <list_>         BLOCK IF_BLOCK BIG_BLOCK
%type   <str>           OBJECT_INHERITENCE

%type   <integer>       DECL_QUALIFIER
%type   <type_info>     TYPE

/* Expression */
%type   <expression>    EXPRESSION
%type   <expression>    IDENT_EXPRESSION
%type   <expression>    ARITHMETIC_EXPRESSION
%type   <expression>    PLUS_EXPRESSION
%type   <expression>    MINUS_EXPRESSION
%type   <expression>    MUL_EXPRESSION
%type   <expression>    DIV_EXPRESSION
%type   <expression>    ORDER_EXPRESSION
%type   <expression>    LESS_THAN_EXPRESSION
%type   <expression>    GREATER_THAN_EXPRESSION
%type   <expression>    EQUAL_EXPRESSION
%type   <expression>    LESS_OR_EQUAL_EXPRESSION
%type   <expression>    GREATER_OR_EQUAL_EXPRESSION
%type   <expression>    NOT_EQUAL_EXPRESSION
%type   <expression>    DOT_EXPRESSION
%type   <expression>    FUNCTION_CALL_EXPRESSION
%type   <expression>    ASSIGNMENT_EXPRESSION
%type   <expression>    CONSTANT_EXPRESSION
%type   <expression>    INDEX_EXPRESSION
%type   <expression>    ARRAY_EXPRESSION
%type   <expression>    PERCENT_EXPRESSION

/* Misc */
%type   <list_> VAR_DECL_LIST
%type   <expression>    VAR_DECL
%type   <objDeclBody>   OBJECT_DEF
%type   <objDeclItem>   OBJECT_DEF_ITEM
%type   <list_>         EXPRESSION_LIST
%type   <list_>         ARRAY_ELEMENTS
%type   <list_>         IMPORT_LIST
%type   <list_>         IDENTIFIER_LIST
%type   <list_>         ARGUMENTS PARAMETERS
%type   <expression>    MEMBER_SELECTION_ENTITY
%type   <str>       PARAMETER

%printer { fprintf(yyoutput, "%d", $$->type); } EXPRESSION;
%printer { fprintf(yyoutput, "%s", $$); } IDENTIFIER;


%code top {
    #include "list.h"
    #include "program.h"
    #include "statement.h"
    #include "expression.h"
    #include "object.h"
    #include "func.h"
    #include "pair.h"

    #define YYDEBUG 1

    static void *ref_$$;
}

%%

/* type : Program */
PROGRAM :
    STATEMENTS {
        $$ = programGenerate($STATEMENTS);
        $$->compute($$);
    };

/* With left-recursion grammar to make
 * sure the second rule be match before
 * the frist rule
 *
 * type : list<Statement> */
STATEMENTS :
    STATEMENTS STATEMENT {
        listAppend($$, (void *)$STATEMENT);
    }
    | STATEMENT {
        $$ = listCreate();
        listAppend($$, (void *)$STATEMENT);
    };

/* type : Statement */
STATEMENT :
    IF_STATEMENT
    | OBJECT_DECL_STATEMENT
    | IMPORT_STATEMENT
    | RETURN_STATEMENT
    | VAR_DECL_STATEMENT
    | FUNC_DECL_STATEMENT
    | EXPRESSION_STATEMENT;

/* type : Statement */
IF_STATEMENT :
    IF OPEN_PAREN EXPRESSION CLOSE_PAREN BLOCK {
        $$ = (Statement *)ifStatementGenerate($EXPRESSION, $BLOCK, NULL);
    }
    | IF OPEN_PAREN EXPRESSION CLOSE_PAREN IF_BLOCK ELSE BLOCK {
        $$ = (Statement *)ifStatementGenerate($EXPRESSION, $IF_BLOCK, $BLOCK);
    };

/* type : list<Statement> */
IF_BLOCK :
    OPEN_CURVE_BRACKET STATEMENTS CLOSE_CURVE_BRACKET { $$ = $STATEMENTS; }
    | IF_STATEMENT_WITH_ELSE {
        $$ = listCreate();
        listAppend($$, (void *)$IF_STATEMENT_WITH_ELSE);
    };

/* type : Statement */
IF_STATEMENT_WITH_ELSE :
    IF OPEN_PAREN EXPRESSION CLOSE_PAREN IF_BLOCK[TRUE] ELSE IF_BLOCK[FALSE] {
        $$ = (Statement *)ifStatementGenerate($EXPRESSION, $TRUE, $FALSE);
    }
    | OBJECT_DECL_STATEMENT
    | IMPORT_STATEMENT
    | RETURN_STATEMENT
    | VAR_DECL_STATEMENT
    | FUNC_DECL_STATEMENT
    | EXPRESSION_STATEMENT

/* type : Statement */
OBJECT_DECL_STATEMENT :
    OBJECT IDENTIFIER OBJECT_INHERITENCE OPEN_CURVE_BRACKET
    OBJECT_DEF
    CLOSE_CURVE_BRACKET SEMICOLON {
        $$ = objDeclStmtGen($IDENTIFIER, $OBJECT_DEF->newMembers,
                            $OBJECT_INHERITENCE, $OBJECT_DEF->overWrites);
    };

/* type : char * */
OBJECT_INHERITENCE:
    BELONG IDENTIFIER {
       $$ = IDENTIFIER;
    }
    | /* empty */ {
        $$ = null;
    };

/* type : ObjectDeclBody */
OBJECT_DEF :
    /* Use to set value within parent */
    OBJECT_DEF OBJECT_DEF_ITEM {
        if ($OBJECT_DEF_ITEM->type == OBJECT_OVER_WRITE) {
            if ($$->overWrites == NULL) $$->overWrites = listCreate();
            listAppend($$->overWrites, (void *)$OBJECT_DEF_ITEM->item);
        } else {
            listAppend($$->newMembers, (void *)$OBJECT_DEF_ITEM->item);
        }
    }
    /* Define a new member for the a new object */
    | OBJECT_DEF_ITEM {
        $$ = objBodyGen();
        if ($OBJECT_DEF_ITEM->type == OBJECT_OVER_WRITE) {
            $$->overWrites = listCreate();
            listAppend($$->overWrites, (void *)$OBJECT_DEF_ITEM->item);
        } else {
            $$->newMembers = listCreate();
            listAppend($$->newMembers, (void *)$OBJECT_DEF_ITEM->item);
        }
    };
/* type : ObjectDeclItem */
OBJECT_DEF_ITEM :
    IDENTIFIER[MEMBER] BELONG IDENTIFIER[PARENT] ASSIGNMENT EXPRESSION SEMICOLON {
        $$ = objItemGen();
        $$->type = OBJECT_OVER_WRITE;
        $$->item = pairGen((void *)$MEMBER, (void *)$PARENT);
    }
    | TYPE IDENTIFIER SEMICOLON {
        $$ = objItemGen();
        $$->type = OBJECT_MEMBER;

        Variable *v = varGen(strdup($IDENTIFIER), varTypeStr2Int(TYPE_INFO_TYPE($TYPE)), NULL);
        $$->item = v;
    };

/* type : Statement */
IMPORT_STATEMENT :
    IMPORT IMPORT_LIST FROM IDENTIFIER {};

/* type : list<char *> */
IMPORT_LIST : IDENTIFIER_LIST { $$ = $IDENTIFIER_LIST; }

IDENTIFIER_LIST :
    IDENTIFIER_LIST COMMA IDENTIFIER {
        listAppend($$, (void *)$IDENTIFIER);
    }
    | IDENTIFIER {
        $$ = listCreate();
        listAppend($$, (void *)$IDENTIFIER);
    }

/* type : Statement */
RETURN_STATEMENT :
    RETURN EXPRESSION SEMICOLON {
        $$ = returnStmtGen($EXPRESSION);
    };

/* type : Statement */
VAR_DECL_STATEMENT :
    TYPE VAR_DECL_LIST SEMICOLON {
        VarDeclStatement *stmt = varDeclStmtGenerate($TYPE);
        VAR_DECL_STMT_SET_EXPRS(stmt, $VAR_DECL_LIST);

        $$ = (Statement *)stmt;
    };

/* type : list<Expression> */
VAR_DECL_LIST:
    VAR_DECL_LIST COMMA VAR_DECL {
        listAppend($$, (void *)$VAR_DECL);
    }
    | VAR_DECL {
        $$ = listCreate();
        listAppend($$, (void *)$VAR_DECL);
    };

/* type : pair(ident, Expression) */
VAR_DECL :
    IDENTIFIER {
        $$ = pairGen($IDENTIFIER, identExprGen($IDENTIFIER));
    }
    | IDENTIFIER ASSIGNMENT EXPRESSION {
        Expression *expr = assignExprGen(identExprGen($IDENTIFIER), $EXPRESSION);
        $$ = pairGen($IDENTIFIER, expr);
    };

/* type : Statement */
FUNC_DECL_STATEMENT :
    TYPE IDENTIFIER OPEN_PAREN PARAMETERS CLOSE_PAREN BIG_BLOCK {
        $$ = funcDeclStmtDefault();

        Func *f = funcGen($IDENTIFIER, $TYPE, $PARAMETERS, NULL);
        FUNC_SET_STATEMENT_LIST(f, $BIG_BLOCK);

        ((FuncDeclStatement *)$$)->f = f;
    }
    | TYPE IDENTIFIER OPEN_PAREN CLOSE_PAREN BIG_BLOCK {
        $$ = funcDeclStmtDefault();

        Func *f = funcGen($IDENTIFIER, $TYPE, NULL, NULL);
        FUNC_SET_STATEMENT_LIST(f, $BIG_BLOCK);

        ((FuncDeclStatement *)$$)->f = f;
    };

/* type : Parameters */
PARAMETERS :
    PARAMETERS COMMA PARAMETER {
        paramsAdd($$, $PARAMETER);
    }
    | PARAMETER {
        $$ = paramsGen;
        paramsAdd($$, $PARAMETER);
    };

/* type : Parameter */
PARAMETER :
    TYPE IDENTIFIER {
        $$ = paramGen($IDENTIFIER, $TYPE);
    };

/* type : Statement */
EXPRESSION_STATEMENT :
    EXPRESSION SEMICOLON {
        $$ = exprStmtGen($EXPRESSION);
    };

TYPE :
    INT DECL_QUALIFIER {
        $$ = buildTypeInfo(strdup("Int"), $DECL_QUALIFIER);
    }
    | STR DECL_QUALIFIER {
        $$ = buildTypeInfo(strdup("String"), $DECL_QUALIFIER);
    }
    | MACHINE_OPERATIONS DECL_QUALIFIER {
        $$ = buildTypeInfo(strdup("Ops"), $DECL_QUALIFIER);
    }
    | IDENTIFIER DECL_QUALIFIER {
        $$ = buildTypeInfo(strdup($IDENTIFIER), $DECL_QUALIFIER);
    };

DECL_QUALIFIER :
    ARRAY_MARK {
        $$ = ARRAY_TYPE;
    }
    | MAP_MARK {
        $$ = MAP_TYPE;
    }
    | /* empty */ {
        $$ = 0;
    };

/* type : list<expression> */
EXPRESSION_LIST :
    EXPRESSION_LIST COMMA EXPRESSION {
        listAppend($$, $EXPRESSION);
    }
    | EXPRESSION {
        $$ = listCreate();
        listAppend($$, $EXPRESSION);
    };

/* type : Expression */
EXPRESSION :
    OPEN_PAREN EXPRESSION[SUB] CLOSE_PAREN {
        $$ = $SUB;
    }
    | ARITHMETIC_EXPRESSION
    | ORDER_EXPRESSION
    | DOT_EXPRESSION
    | FUNCTION_CALL_EXPRESSION
    | ASSIGNMENT_EXPRESSION
    | CONSTANT_EXPRESSION
    | IDENT_EXPRESSION
    | INDEX_EXPRESSION
    | ARRAY_EXPRESSION
    | PERCENT_EXPRESSION;

/* type : Expression */
PERCENT_EXPRESSION :
    EXPRESSION PERCENTAGE OPEN_PAREN EXPRESSION_LIST CLOSE_PAREN {
        $$ = percentExprGen($EXPRESSION, $EXPRESSION_LIST);
    };

/* type : Expression */
ARRAY_EXPRESSION :
    OPEN_CURVE_BRACKET ARRAY_ELEMENTS CLOSE_CURVE_BRACKET {
        $$ = arrayExprGen($ARRAY_ELEMENTS);
    };
/* type : list<expression> */
ARRAY_ELEMENTS : EXPRESSION_LIST;

/* type : Expression */
INDEX_EXPRESSION :
    EXPRESSION[left] OPEN_SQUARE_BRACKET EXPRESSION[right] CLOSE_SQUARE_BRACKET {
        $$ = indexExprGen($left, $right);
    };

/* type : Expression */
IDENT_EXPRESSION :
    IDENTIFIER {
        $$ = identExprGen($IDENTIFIER);
    }

/* type : Expression */
ARITHMETIC_EXPRESSION :
    PLUS_EXPRESSION
    | MINUS_EXPRESSION
    | MUL_EXPRESSION
    | DIV_EXPRESSION;

/* type : Expression */
PLUS_EXPRESSION :
    EXPRESSION[LEFT] PLUS EXPRESSION[RIGHT] {
        $$ = plusExprGen($LEFT, $RIGHT);
    };

/* type : Expression */
MINUS_EXPRESSION :
    EXPRESSION[LEFT] MINUS EXPRESSION[RIGHT] {
        $$ = minusExprGen($LEFT, $RIGHT);
    };

/* type : Expression */
MUL_EXPRESSION :
    EXPRESSION[LEFT] MUL EXPRESSION[RIGHT] {
        $$ = mulExprGen($LEFT, $RIGHT);
    };

/* type : Expression */
DIV_EXPRESSION :
    EXPRESSION[LEFT] DIV EXPRESSION[RIGHT] {
        $$ = divExprGen($LEFT, $RIGHT);
    };

/* type : Expression */
ORDER_EXPRESSION :
    LESS_THAN_EXPRESSION
    | GREATER_THAN_EXPRESSION
    | EQUAL_EXPRESSION
    | LESS_OR_EQUAL_EXPRESSION
    | GREATER_OR_EQUAL_EXPRESSION
    | NOT_EQUAL_EXPRESSION;

/* type : Expression */
LESS_THAN_EXPRESSION :
    EXPRESSION[LEFT] LESS_THAN EXPRESSION[RIGHT] {
        $$ = lessThanExprGen($LEFT, $RIGHT);
    };

/* type : Expression */
GREATER_THAN_EXPRESSION :
    EXPRESSION[LEFT] GREATER_THAN EXPRESSION[RIGHT] {
        $$ = greaterThanExprGen($LEFT, $RIGHT);

    };

/* type : Expression */
EQUAL_EXPRESSION :
    EXPRESSION[LEFT] EQUAL EXPRESSION[RIGHT] {
        $$ = equalExprGen($LEFT, $RIGHT);
    };

/* type : Expression */
LESS_OR_EQUAL_EXPRESSION :
    EXPRESSION[LEFT] LESS_OR_EQUAL EXPRESSION[RIGHT] {
        $$ = lessOrEqualExprGen($LEFT, $RIGHT);
    };

/* type : Expression */
GREATER_OR_EQUAL_EXPRESSION :
    EXPRESSION[LEFT] GREATER_OR_EQUAL EXPRESSION[RIGHT] {
        $$ = greaterOrEqualGen($LEFT, $RIGHT);
    };

/* type : Expression */
NOT_EQUAL_EXPRESSION :
    EXPRESSION[LEFT] NOT_EQUAL EXPRESSION[RIGHT] {
        $$ = notEqualExprGen($LEFT, $RIGHT);
    };

/* type : Expression */
DOT_EXPRESSION :
    DOT_EXPRESSION IDENTIFIER {
        MemberSelectExpression *mExpr = $$;
        if (MEMBER_SELECT_SUBS(mExpr) == NULL)
            MEMBER_SELECT_SET_SUBS(mExpr, $IDENTIFIER);
        else {
            Expression *m = $$;
            $$ = memberSelectGen(m, $IDENTIFIER);
        }
    }
    | MEMBER_SELECTION_ENTITY {
        $$ = memberSelectGen($MEMBER_SELECTION_ENTITY, NULL);
    };

/* type : Expression */
MEMBER_SELECTION_ENTITY :
    IDENTIFIER DOT {
        $$ = identExprGen($IDENTIFIER);
    }
    | FUNCTION_CALL_EXPRESSION DOT {
        $$ = $FUNCTION_CALL_EXPRESSION;
    };

/* type : Expression */
FUNCTION_CALL_EXPRESSION :
    IDENTIFIER OPEN_PAREN ARGUMENTS CLOSE_PAREN {
        $$ = funcCallExprGen($IDENTIFIER, $ARGUMENTS);
    }
    | IDENTIFIER OPEN_PAREN CLOSE_PAREN {
        $$ = funcCallExprGen($IDENTIFIER, NULL);
    };

/* type : Arguments */
ARGUMENTS :
    ARGUMENTS COMMA EXPRESSION {
        argusAdd($$, arguGen($EXPRESSION));
    }
    | EXPRESSION {
        $$ = argusGen();
        argusAdd($$, arguGen($EXPRESSION));
    };

/* Statement */
ASSIGNMENT_EXPRESSION :
    EXPRESSION[LEFT] ASSIGNMENT EXPRESSION[RIGHT] {
        $$ = assignExprGen($LEFT, $RIGHT);
    };

/* type : list<Statement> */
BLOCK :
    BIG_BLOCK {
        $$ = $BIG_BLOCK;
    }
    | STATEMENT {
        $$ = listCreate();
        listAppend($$, $STATEMENT);
    };

/* type : list<Statement> */
BIG_BLOCK :
    OPEN_CURVE_BRACKET STATEMENTS CLOSE_CURVE_BRACKET {
        $$ = $STATEMENTS;
    };

/* type : Expression */
CONSTANT_EXPRESSION :
    NUM {
        $$ = constExprDefault();
        constExprSetInt($$, str2Num($NUM));
    }
    | STR_LITERAL {
        $$ = constExprDefault();
        constExprSetStr($$, $STR_LITERAL);
    };

/* type : Expression */

%%

void yyerror(char const *s) {
    fprintf(stderr, "%s\n", s);
}
