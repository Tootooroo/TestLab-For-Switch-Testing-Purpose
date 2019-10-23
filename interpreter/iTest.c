/* iTest.c */

#include <stdio.h>
#include "module.h"
#include "variable_ops.h"

extern int yylex(void);
extern FILE *yyin;
extern int yyparse(void);
extern int yydebug;

int main(void) {
    yydebug = 0;

    yyin = fopen("sample", "r");

    variableOpsInit();
    moduleInit();

    yyparse();

    fclose(yyin);

    return 0;
}
