%union {
    char *str;
    int integer;
    list *list_;
    Program *program;
    Statements *statements;
    Statement *statement;
    Expression *expression;

    ObjectDeclBody *objDeclBody;
    ObjectDeclItem *objDeclItem;
}

%token IMPORT FROM
%token RETURN
%token OBJECT
%token BELONG

%token MACHINE_OPERATIONS

%token IF ELSE

%token COMMA COLON SEMICOLON
%token OPEN_CURVE_BRACKET CLOSE_CURVE_BRACKET

%token INT
%token STR

%token <str> IDENTIFIER
%token <integer> NUM
%token <str> STR_LITERAL

%left EQUAL LESS_THAN GREATER_THAN LESS_OR_EQUAL GREATER_OR_EQUAL NOT_EQUAL
%left PLUS MINUS
%left MUL DIV
%right ASSIGNMENT
%left OPEN_PAREN CLOSE_PAREN
%left DOT OPEN_SQUARE_BRACKET CLOSE_SQUARE_BRACKET

/* Statements */
%type   <program>       PROGRAM
%type   <statements>    STATEMENTS
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
%type   <str>           TYPE

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
%type   <expression>    MEMBER_SELECTION_EXPRESSION
%type   <expression>    FUNCTION_CALL_EXPRESSION
%type   <expression>    ASSIGNMENT_EXPRESSION
%type   <expression>    CONSTANT_EXPRESSION

/* Misc */
%type   <VAR_DECL_LIST> VAR_DECL_LIST
%type   <expression>    VAR_DECL
%type   <objDeclBody>   OBJECT_DEF
%type   <objDeclItem>   OBJECT_DEF_ITEM
%type   <list_>         IMPORT_LIST
%type   <list_>         ARGUMENTS PARAMETERS
%type   <expression>    MEMBER_SELECTION_ENTITY
%type   <str>       PARAMETER

%code top { }

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
    OPEN_CURVE_BRACKET STATEMENTS CLOSE_CURVE_BRACKET { $$ = STATEMENTS; }
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
    OBJECT IDENTIFIER OBJECT_INHERITENCE OPEN_CURVE_BRACKET OBJECT_DEF CLOSE_CURVE_BRACKET SEMICOLON {
        $$ = objDeclStmtGen($IDENTIFIER, $OBJECT_DEF.newMembers, $OBJECT_INHERITENCE, $OBJECT_DEF.overWrites);
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
        if ($OBJECT_DEF_ITEM->type == 0) {
            listAppend($$->overWrites, (void *)$OBJECT_DEF_ITEM->item);
        } else {
            listAppend($$->overWrites, (void *)$OBJECT_DEF_ITEM->item);
        }
    }
    /* Define a new member for the a new object */
    | OBJECT_DEF_ITEM {
        $$ = objBodyGen();
        if ($OBJECT_DEF_ITEM->type == OBJECT_OVER_WRITE) {
            $$->overwrites = listCreate();
            listJoin($$->overWrites, (void *)$OBJECT_DEF_ITEM->item);
        } else {
            $$->newMembers = listCreate();
            listJoin($$->newMembers, (void *)$OBJECT_DEF_ITEM->item);
        }
    };
/* type : ObjectDeclItem */
OBJECT_DEF_ITEM :
    IDENTIFIER[MEMBER] BELONG IDENTIFIER[PARENT] ASSIGNMENT EXPRESSION SEMICOLON {
        $$ = objItemGen();
        $$->type = OBJECT_OVER_WRITE;
        $$->item = pairGenerate((void *)$MEMBER, (void *)$PARENT);
    }
    | TYPE IDENTIFIER SEMICOLON {
        $$ = objItemGen();
        $$->type = OBJECT_MEMBER;
        $$->item = pairGen((void *)$IDENTIFIER, varGen(strdup($IDENTIFIER), varTypeStr2Int($TYPE), NULL));
    };

/* type : Statement */
IMPORT_STATEMENT :
    IMPORT IMPORT_LIST FROM IDENTIFIER {
        $$ = importStmtDefault();
        $$->importSymbols = $IMPORT_LIST;
        $$->from = $IDENTIFIER;
    };

/* type : list<char *> */
IMPORT_LIST :
    IMPORT_LIST COMMA IDENTIFIER {
        listAppend($$, (void *)$IDENTIFIER);
    }
    | IDENTIFIER {
        $$ = listCreate();
        listAppend($$, (void *)$IDENTIFIER);
    };

/* type : Statement */
RETURN_STATEMENT :
    RETURN EXPRESSION SEMICOLON {
        $$ = returnStmtGen($EXPRESSION);
    };

/* type : Statement */
VAR_DECL_STATEMENT :
    TYPE VAR_DECL_LIST SEMICOLON {
        $$ = varDeclStmtGenerate($VAR_DECL_LIST);
    };
/* type : list<Expression> */
VAR_DECL_LIST :
    VAR_DECL_LIST COMMA VAR_DECL {
        listAppend($$, (void *)$VAR_DECL);
    }
    | VAR_DECL {
        $$ = listCreate();
        listAppend($$, (void *)$VAR_DECL);
    };

/* type : Expression */
VAR_DECL :
    IDENTIFIER {
        $$ = identExprGen($IDENTIFIER);
    }
    | IDENTIFIER ASSIGNMENT EXPRESSION {
        $$ = assignExprGen(identExprGen($IDENTIFIER), $EXPRESSION);
    };

/* type : Statement */
FUNC_DECL_STATEMENT :
    TYPE IDENTIFIER OPEN_PAREN PARAMETERS CLOSE_PAREN BIG_BLOCK {
        $$ = funcDeclStmtDefault();

        Func *f = funcGen($IDENTIFIER, $TYPE, PARAMETERS, NULL);
        FUNC_SET_STATMENT_LIST(f, $BIG_BLOCK);

        $$->f = f;
    }
    | TYPE IDENTIFIER OPEN_PAREN CLOSE_PAREN BIG_BLOCK {
        $$ = funcDeclStmtDefault();

        Func *f = funcGen($IDENTIFIER, $TYPE, NULL, NULL);
        FUNC_SET_STATEMENT_LIST(f, $BIG_BLOCK);

        $$->f = f;
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
        $$ = strdup("Int");
    }
    | STR DECL_QUALIFIER {
        $$ = strdup("String");
    }
    | MACHINE_OPERATIONS DECL_QUALIFIER {
        $$ = strdup("Ops");
    }
    | IDENTIFIER DECL_QUALIFIER {
        $$ = strdup($IDENTIFIER);
    };

DECL_QUALIFIER :
    ARRAY_QUALIFIER {
        $$ = 0;
    }
    | /* empty */ {
        $$ = 1;
    };
ARRAY_QUALIFIER :
    OPEN_SQUARE_BRACKET CLOSE_SQUARE_BRACKET;

/* type : Expression */
EXPRESSION :
    OPEN_PAREN EXPRESSION[SUB] CLOSE_PAREN {
        $$ = $SUB;
    }
    | ARITHMETIC_EXPRESSION
    | ORDER_EXPRESSION
    | MEMBER_SELECTION_EXPRESSION
    | FUNCTION_CALL_EXPRESSION
    | ASSIGNMENT_EXPRESSION
    | CONSTANT_EXPRESSION
    | IDENT_EXPRESSION;

/* type : Expression */
IDENT_EXPRESSION :
    IDENTIFIER {
        identExprGen($IDENTIFIER);
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
        $$ = plusStmtGen($LEFT, $RIGHT);
    };

/* type : Expression */
MINUS_EXPRESSION :
    EXPRESSION[LEFT] MINUS EXPRESSION[RIGHT] {
        $$ = minusStmtGen($LEFT, $RIGHT);
    };

/* type : Expression */
MUL_EXPRESSION :
    EXPRESSION[LEFT] MUL EXPRESSION[RIGHT] {
        $$ = mulStmtGen($LEFT, $RIGHT);
    };

/* type : Expression */
DIV_EXPRESSION :
    EXPRESSION[LEFT] DIV EXPRESSION[RIGHT] {
        $$ = divStmtGen($LEFT, $RIGHT);
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
        $$ = notEqualExprGeen($LEFT, $RIGHT);
    };

/* type : Expression */
MEMBER_SELECTION_EXPRESSION :
    MEMBER_SELECTION_EXPRESSION IDENTIFIER {
        listAppend($$->subs, identExprGen($IDENTIFIER));
    }
    | MEMBER_SELECTION_EXPRESSION MEMBER_SELECTION_ENTITY {
        listAppend($$->subs, $MEMBER_SELECTION_ENTITY);
    }
    | MEMBER_SELECTION_ENTITY {
        $$ = memberSelectDefault();
        $$->subs = listCreate();
        $$->head = $MEMBER_SELECTION_ENTITY;
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
        constExprSetInt($$, str2Int($NUM));
    }
    | STR_LITERAL {
        $$ = constExprDefault();
        constExprSetStr($$, $STR_LITERAL);
    };

%%
