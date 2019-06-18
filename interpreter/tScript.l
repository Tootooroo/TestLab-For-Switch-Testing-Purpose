%option header-file= "interpreter/lexer.h"
%option outfile= "src/lexer.c"
%option yylineno

%{ %}

%%

[\n ]+    { /* empty */ }
[\t ]+    { /* empty */ }

import    { return IMPORT; }
return    { return RETURN; }

Object    { return OBJECT; }
::        { return BELONG; }

Int       { return INT; }
String    { return STR; }
Ops       { return MACHINE_OPERATIONS; }

if        { return IF; }
else      { return ELSE; }

+         { return PLUS; }
-         { return MINUS; }
*         { return MUL; }
/         { return DIV; }

<         { return LESS_THAN; }
>         { return GREATER_THAN; }
==        { return EQUAL; }
<=        { return LESS_OR_EQUAL; }
>=        { return GREATER_OR_EQUAL; }
!=        { return NOT_EQUAL; }

=         { return ASSIGNMENT; }

\,        { return COMMA; }
\:        { return colon;  }
\;        { return semisolon; }
\.        { return DOT; }

\(        { return OPEN_PAREN; }
\)        { return CLOSE_PAREN; }
\[        { return OPEN_SQUARE_BRACKET; }
\]        { return CLOSE_SQUARE_BRACKET; }
\{        { return OPEN_CURVE_BRACKET; }
\}        { return CLOSE_CURVE_BRACKET; }

"[^"]*"   { return STR_LITERAL; }
[0-9]+    { return NUM; }
([a-zA-Z0-9]|"-")+     { yylval.str = strdup(yytext); return IDENTIFIER; }

%%

int yywrap() {
    return 1;
}