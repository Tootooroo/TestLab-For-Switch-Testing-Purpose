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
%type   <str>       TYPE

/* Expression */
%type   <expression>    EXPRESSION
%type   <expression>    PLUS_EXPRESSION
%type   <expression>    MINUS_EXPRESSION
%type   <expression>    MUL_EXPRESSION
%type   <expression>    DIV_EXPRESSION
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
%type   <list_>         ARGUMENT_LIST PARAMETER_LIST
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
        $$ = objDeclStmtDefault();
        $$->objectType = $IDENTIFIER;
        if ($OBJECT_INHERITENCE) {
            /* Deal with inheritence */
            $$->parent = $OBJECT_INHERITENCE;
            $$->overWrites = $OBJECT_DEF.overWrites;
        }
        $$->members = $OBJECT_DEF.newMembers;
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
        if ($OBJECT_DEF_ITEM->type == 0) {
            $$->overwrites = listCreate();
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
        $$->type = 0;
        $$->item = pairGenerate((void *)$MEMBER, (void *)$PARENT);
    }
    | TYPE IDENTIFIER SEMICOLON {
        $$ = objItemGen();
        $$->type = 1;
        $$->item = pairGenerate((void *)$IDENTIFIER, $TYPE);
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
    STR_LITERAL {
        $$ = (Expression *)constExprDefault();
        CONSTANT_SET_STR((ConstantExpression *)$$, $STR_LITERAL);
    }
    | NUM {
        $$ = (Expression *)constExprDefault();
        /* fixme: should transfer $NUM from char * into int */
        CONSTANT_SET_INT((ConstantExpression *)$$, $NUM);
    }
    | ASSIGNMENT_EXPRESSION {
        $$ = (Expression *)$ASSIGNMENT_EXPRESSION;
    };

/* type : Statement */
FUNC_DECL_STATEMENT :
    TYPE IDENTIFIER OPEN_PAREN PARAMETER_LIST CLOSE_PAREN BIG_BLOCK {
        $$ = funcDeclStmtDefault();

        Func *f = funcGenerate();
        FUNC_SET_IDENT(f, $IDENTIFIER);
        FUNC_SET_RETURN_TYPE(f, $TYPE);
        FUNC_SET_STATMENT_LIST(f, $BIG_BLOCK);
        FUNC_SET_PARAMETER_LIST(f, $PARAMETER_LIST);

        $$->f = f;
    }
    | TYPE IDENTIFIER OPEN_PAREN CLOSE_PAREN BIG_BLOCK {
        $$ = funcDeclStmtDefault();

        Func *f = funcGenerate();
        FUNC_SET_IDENT(f, $IDENTIFIER);
        FUNC_SET_RETURN_TYPE(f, $TYPE);
        FUNC_SET_STATEMENT_LIST(f, $BIG_BLOCK);
        FUNC_SET_STATEMENT_LIST(f, null);

        $$->f = f;
    };

/* type : list<type :: int> */
PARAMETER_LIST :
    PARAMETER_LIST COMMA PARAMETER {

    }
    | PARAMETER {

      };
PARAMETER :
    TYPE IDENTIFIER {

    }
    | TYPE {

    };

/* type : Statement */
EXPRESSION_STATEMENT :
    EXPRESSION SEMICOLON {
        $$ = exprStmtGen($EXPRESSION);
    };

TYPE :
    INT DECL_QUALIFIER
    | STR DECL_QUALIFIER
    | MACHINE_OPERATIONS ARRAY_QUALIFIER
    | IDENTIFIER ARRAY_QUALIFIER;

DECL_QUALIFIER :
    ARRAY_QUALIFIER;
ARRAY_QUALIFIER :
    OPEN_SQUARE_BRACKET CLOSE_SQUARE_BRACKET

/* type : Expression */
EXPRESSION :
    OPEN_PAREN EXPRESSION CLOSE_PAREN
    | ARITHMETIC_EXPRESSION
    | ORDER_EXPRESSION
    | MEMBER_SELECTION_EXPRESSION
    | FUNCTION_CALL_EXPRESSION
    | ASSIGNMENT_EXPRESSION
    | CONSTANT_EXPRESSION
    | IDENTIFIER;

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
    EXPRESSION EQUAL EXPRESSION {

    };

/* type : Expression */
LESS_OR_EQUAL_EXPRESSION :
    EXPRESSION LESS_OR_EQUAL EXPRESSION;

/* type : Expression */
GREATER_OR_EQUAL_EXPRESSION :
    EXPRESSION GREATER_OR_EQUAL EXPRESSION;

/* type : Expression */
NOT_EQUAL_EXPRESSION :
    EXPRESSION NOT_EQUAL EXPRESSION;

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
    IDENTIFIER OPEN_PAREN ARGUMENT_LIST CLOSE_PAREN {
        $$ = funcCallExprGen($IDENTIFIER, $ARGUMENT_LIST);
    }
    | IDENTIFIER OPEN_PAREN CLOSE_PAREN {
        $$ = funcCallExprGen($IDENTIFIER, NULL);
    };

/* type : list<Expression> */
ARGUMENT_LIST :
    ARGUMENT_LIST COMMA EXPRESSION {
        listAppend($$, $EXPRESSION);
    }
    | EXPRESSION {
        $$ = listCreate();
        listAppend($$, $EXPRESSION);
    };

/* Statement */
ASSIGNMENT_EXPRESSION :
    EXPRESSION[LEFT] ASSIGNMENT EXPRESSION[RIGHT] {
        $$ = assignExprGen($LEFT, $RIGHT);
    };

/* type : list<Statement> */
BLOCK :
    BIG_BLOCK
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
