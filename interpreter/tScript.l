%option header-file= "interpreter/lexer.h"
%option outfile= "interpreter/lexer.c"
%option yylineno

%{
    #include "list.h"
    #include "program.h"
    #include "statement.h"
    #include "expression.h"
    #include "object.h"
    #include "misc.h"

    #include "parser.h"
%}

%%

[\n ]+    { /* empty */ }
[\t ]+    { /* empty */ }
--.*      { /* empty */ }

import    { return IMPORT; }
return    { return RETURN; }

Object    { return OBJECT; }
::        { return BELONG; }

Int       { return INT; }
String    { return STR; }
Ops       { return MACHINE_OPERATIONS; }

if        { return IF; }
else      { return ELSE; }

\@        { return ARRAY_MARK; }
\#        { return MAP_MARK; }

\%        { return PERCENTAGE; }
\+         { return PLUS; }
\-         { return MINUS; }
\*         { return MUL; }
\/         { return DIV; }

\<         { return LESS_THAN; }
\>         { return GREATER_THAN; }
==         { return EQUAL; }
\<=        { return LESS_OR_EQUAL; }
\>=        { return GREATER_OR_EQUAL; }
\!=        { return NOT_EQUAL; }

=         { return ASSIGNMENT; }

\,        { return COMMA; }
\;        { return SEMICOLON; }
\.        { return DOT; }

\(        { return OPEN_PAREN; }
\)        { return CLOSE_PAREN; }
\[        { return OPEN_SQUARE_BRACKET; }
\]        { return CLOSE_SQUARE_BRACKET; }
\{        { return OPEN_CURVE_BRACKET; }
\}        { return CLOSE_CURVE_BRACKET; }

\"([^\"]|(\\\"))*[^\\]\"     { yylval.str = strdup(yytext); return STR_LITERAL; }

[0-9]+                { yylval.str = strdup(yytext); return NUM; }

([a-zA-Z0-9]|"-")+    { yylval.str = strdup(yytext); return IDENTIFIER; }

%%

int yywrap() {
    return 1;
}
