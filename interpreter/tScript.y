%union {
    char *str;
    int integer;
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


%code top { }

%%

PROGRAM :
    STATEMENTS;

STATEMENTS :
    STATEMENT STATEMENTS
    | /* empty */;

STATEMENT :
    IF_STATEMENT
    | OBJECT_DECL_STATEMENT
    | IMPORT_STATEMENT
    | RETURN_STATEMENT
    | VAR_DECL_STATEMENT
    | FUNC_DECL_STATEMENT
    | EXPRESSION_STATEMENT;

IF_STATEMENT :
    IF OPEN_PAREN EXPRESSION CLOSE_PAREN BLOCK
    | IF OPEN_PAREN EXPRESSION CLOSE_PAREN IF_BLOCK ELSE BLOCK;

IF_BLOCK :
    OPEN_CURVE_BRACKET STATEMENTS CLOSE_CURVE_BRACKET
    | IF_STATEMENT_WITH_ELSE;
IF_STATEMENT_WITH_ELSE :
    IF OPEN_PAREN EXPRESSION CLOSE_PAREN IF_BLOCK ELSE IF_BLOCK
    | OBJECT_DECL_STATEMENT
    | IMPORT_STATEMENT
    | RETURN_STATEMENT
    | VAR_DECL_STATEMENT
    | FUNC_DECL_STATEMENT
    | EXPRESSION_STATEMENT

OBJECT_DECL_STATEMENT :
    OBJECT IDENTIFIER OBJECT_INHERITENCE OPEN_CURVE_BRACKET OBJECT_DEF CLOSE_CURVE_BRACKET SEMICOLON;
OBJECT_INHERITENCE:
    BELONG IDENTIFIER
OBJECT_DEF :
    /* Use to set value within parent */
    IDENTIFIER BELONG IDENTIFIER ASSIGNMENT EXPRESSION SEMICOLON OBJECT_DEF
    /* Define a new member for the a new object */
    | TYPE IDENTIFIER SEMICOLON OBJECT_DEF
    | /* empty */;

IMPORT_STATEMENT :
    IMPORT IMPORT_LIST FROM IDENTIFIER
IMPORT_LIST :
    IDENTIFIER IMPORT_LIST
    | COMMA IDENTIFIER IMPORT_LIST
    | /* empty */

RETURN_STATEMENT :
    RETURN EXPRESSION SEMICOLON;

VAR_DECL_STATEMENT :
    TYPE VAR_DECL_LIST SEMICOLON;
VAR_DECL_LIST :
    VAR_DECL COMMA VAR_DECL_LIST
    | VAR_DECL;
VAR_DECL :
    IDENTIFIER
    | ASSIGNMENT_EXPRESSION;

FUNC_DECL_STATEMENT :
    TYPE IDENTIFIER OPEN_PAREN ARGUMENT_LIST CLOSE_PAREN SEMICOLON;

EXPRESSION_STATEMENT :
    EXPRESSION SEMICOLON;

TYPE :
    INT
    | STR
    | MACHINE_OPERATIONS
    | IDENTIFIER;

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
