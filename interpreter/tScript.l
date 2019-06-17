%option header-file= "interpreter/lexer.h"
%option outfile= "src/lexer.c"
%option yylineno

%{ %}

%%

[\n ]+    { /* empty */ }
[\t ]+    { /* empty */ }

import    { return IMPORT; }
Object    { return OBJECT; }
::        { return BELONG; }

Int       { return INT; }
String    { return STR; }
Ops       { return MACHINE_OPERATIONS; }

=       { return ASSIGNMENT; }

\,        { return COMMA; }
\:        { return colon;  }
\;        { return semisolon; }
\.        { return DOT; }

\(        { return OPEN_BRACKET; }
\)        { return CLOSE_BRACKET; }
\[        { return OPEN_SQUARE_BRACKET; }
\]        { return CLOSE_SQUARE_BRACKET; }
\{        { return OPEN_CURVE_BRACKET; }
\}        { return CLOSE_CURVE_BRACKET; }

[0-9]+    { return NUM; }
([a-zA-Z0-9]|"-")+     { yylval.str = strdup(yytext); return IDENTIFIER; }

%%

int yywrap() {
    return 1;
}