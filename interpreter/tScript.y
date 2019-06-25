%union {
    char *str;
    int integer;
    list *list_;
    Program *program;
    Statements *statements;
    Statement *statement;
    Expression *expression;
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

%type   <statement>     BLOCK IF_BLOCK
%type   <str>           OBJECT_INHERITENCE

%type   <integer>       DECL_QUALIFIER
%type   <integer>       TYPE

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

/* Misc */
%type   <VAR_DECL_LIST> VAR_DECL_LIST
%type   <expression>    VAR_DECL

%code top { }

%%

PROGRAM :
    STATEMENTS {
        $$ = programGenerate($STATEMENTS);
        $$->compute($$);
    };

STATEMENTS :
    STATEMENTS STATEMENT {
        listAppend($$, (void *)$STATEMENT);
    }
    | STATEMENT {
        $$ = listCreate();
        listAppend($$, (void *)$STATEMENT);
    };

STATEMENT :
    IF_STATEMENT
    | OBJECT_DECL_STATEMENT
    | IMPORT_STATEMENT
    | RETURN_STATEMENT
    | VAR_DECL_STATEMENT
    | FUNC_DECL_STATEMENT
    | EXPRESSION_STATEMENT;

IF_STATEMENT :
    IF OPEN_PAREN EXPRESSION CLOSE_PAREN BLOCK {
        $$ = (Statement *)ifStatementGenerate($EXPRESSION, $BLOCK, NULL);
    }
    | IF OPEN_PAREN EXPRESSION CLOSE_PAREN IF_BLOCK ELSE BLOCK {
        $$ = (Statement *)ifStatementGenerate($EXPRESSION, $IF_BLOCK, $BLOCK);
    };

IF_BLOCK :
    OPEN_CURVE_BRACKET STATEMENTS CLOSE_CURVE_BRACKET { $$ = STATEMENTS; }
    | IF_STATEMENT_WITH_ELSE {
        $$ = listCreate();
        listAppend($$, (void *)$IF_STATEMENT_WITH_ELSE);
    };
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
OBJECT_INHERITENCE:
    BELONG IDENTIFIER {
       $$ = IDENTIFIER;
    }
    | /* empty */ {
        $$ = null;
    };

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

IMPORT_STATEMENT :
    IMPORT IMPORT_LIST FROM IDENTIFIER {
        $$ = importStmtDefault();
        $$->importSymbols = $IMPORT_LIST;
        $$->from = $IDENTIFIER;
    };
IMPORT_LIST :
    IMPORT_LIST COMMA IDENTIFIER {
        listAppend($$, (void *)$IDENTIFIER);
    }
    | IDENTIFIER {
        $$ = listCreate();
        listAppend($$, (void *)$IDENTIFIER);
    };

RETURN_STATEMENT :
    RETURN EXPRESSION SEMICOLON {
        $$ = returnStmtGen($EXPRESSION);
    };

VAR_DECL_STATEMENT :
    TYPE VAR_DECL_LIST SEMICOLON {
        $$ = varDeclStmtGenerate($VAR_DECL_LIST);
    };
VAR_DECL_LIST :
    VAR_DECL_LIST COMMA VAR_DECL {
        listAppend($$, (void *)$VAR_DECL);
    }
    | VAR_DECL {
        $$ = listCreate();
        listAppend($$, (void *)$VAR_DECL);
    };
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
        $$ = (Expression *)
    };

FUNC_DECL_STATEMENT :
    TYPE IDENTIFIER OPEN_PAREN ARGUMENT_LIST CLOSE_PAREN SEMICOLON;

EXPRESSION_STATEMENT :
    EXPRESSION SEMICOLON;

TYPE :
    INT DECL_QUALIFIER
    | STR DECL_QUALIFIER
    | MACHINE_OPERATIONS ARRAY_QUALIFIER
    | IDENTIFIER ARRAY_QUALIFIER;

DECL_QUALIFIER :
    ARRAY_QUALIFIER;
ARRAY_QUALIFIER :
    OPEN_SQUARE_BRACKET CLOSE_SQUARE_BRACKET

EXPRESSION :
    OPEN_PAREN EXPRESSION CLOSE_PAREN
    | ARITHMETIC_EXPRESSION
    | ORDER_EXPRESSION
    | MEMBER_SELECTION_EXPRESSION
    | FUNCTION_CALL_EXPRESSION
    | ASSIGNMENT_EXPRESSION
    | CONSTANT
    | IDENTIFIER;

ARITHMETIC_EXPRESSION :
    PLUS_EXPRESSION
    | MINUS_EXPRESSION
    | MUL_EXPRESSION
    | DIV_EXPRESSION;

PLUS_EXPRESSION :
    EXPRESSION PLUS EXPRESSION;

MINUS_EXPRESSION :
    EXPRESSION MINUS EXPRESSION;

MUL_EXPRESSION :
    EXPRESSION MUL EXPRESSION;

DIV_EXPRESSION :
    EXPRESSION DIV EXPRESSION;

ORDER_EXPRESSION :
    LESS_THAN_EXPRESSION
    | GREATER_THAN_EXPRESSION
    | EQUAL_EXPRESSION
    | LESS_OR_EQUAL_EXPRESSION
    | GREATER_OR_EQUAL_EXPRESSION
    | NOT_EQUAL_EXPRESSION;

LESS_THAN_EXPRESSION :
    EXPRESSION LESS_THAN EXPRESSION;

GREATER_THAN_EXPRESSION :
    EXPRESSION GREATER_THAN EXPRESSION;

EQUAL_EXPRESSION :
    EXPRESSION EQUAL EXPRESSION;

LESS_OR_EQUAL_EXPRESSION :
    EXPRESSION LESS_OR_EQUAL EXPRESSION;

GREATER_OR_EQUAL_EXPRESSION :
    EXPRESSION GREATER_OR_EQUAL EXPRESSION;

NOT_EQUAL_EXPRESSION :
    EXPRESSION NOT_EQUAL EXPRESSION;

MEMBER_SELECTION_EXPRESSION :
    MEMBER_SELECTION_ENTITY DOT IDENTIFIER;
MEMBER_SELECTION_ENTITY :
    IDENTIFIER
    | FUNCTION_CALL_EXPRESSION;

FUNCTION_CALL_EXPRESSION :
    IDENTIFIER OPEN_PAREN ARGUMENT_LIST CLOSE_PAREN ;

ARGUMENT_LIST :
    EXPRESSION
    | EXPRESSION COMMA ARGUMENT_LIST;

ASSIGNMENT_EXPRESSION :
    ASSIGNMENT_LVAL ASSIGNMENT EXPRESSION;
ASSIGNMENT_LVAL :
    IDENTIFIER
    | MEMBER_SELECTION_EXPRESSION;

BLOCK :
    OPEN_CURVE_BRACKET STATEMENTS CLOSE_CURVE_BRACKET
    | STATEMENT;

CONSTANT : NUM | STR_LITERAL;

%%
