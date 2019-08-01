/* iTest.c */

#include <stdio.h>
#include "module.h"

extern int yylex(void);
extern FILE *yyin;
extern int yyparse(void);

int main(void) {
    yyin = fopen("sample", "r");

    moduleInit();

    yyparse();

    fclose(yyin);

    return 0;
}
