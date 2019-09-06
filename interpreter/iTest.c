/* iTest.c */

#include <stdio.h>
#include "module.h"

extern int yylex(void);
extern FILE *yyin;
extern int yyparse(void);
extern int yydebug;

int main(void) {
    yydebug = 0;

    yyin = fopen("sample", "r");

    moduleInit();

    yyparse();

    fclose(yyin);

    return 0;
}
