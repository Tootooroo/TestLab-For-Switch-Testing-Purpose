%union {
    char *str;
    int integer;
}

%token IMPORT
%token OBJECT
%token BELONG

%token MACHINE_OPERATIONS
%token ASSIGNMENT

%token COMMA COLON SEMICOLON DOT
%token OPEN_PAREN CLOSE_PAREN OPEN_SQUARE_BRACKET CLOSE_SQUARE_BRACKET
%token OPEN_CURVE_BRACKET CLOSE_CURVE_BRACKET

%token INT
%token STR

%token <str> IDENTIFIER
%token <integer> NUM

%code top { }

%%

PROGRAM : STATEMENTS

STATEMENTS :  STATEMENT STATEMENTS

STATEMENT : IF_STATEMENT
            | RETURN_STATEMENT
            | VAR_DECL_STATEMENT
            | FUNC_DECL_STATEMENT
            | EXPRESSION_STATEMENT
            | ASSIGNMENT_STATEMENT

IF_STATEMENT :
