/* misc.c */

#include <string.h>

#include "type.h"
#include "misc.h"

#include "list.h"

#define BACK_SLASH '\\'
#define NEWLINE    '\n'
#define QUOTA      '\"'

/* Private Prototypes */
private char * newLineProcessing(char *, char **, list *);
private char * quotaProcessing(char *str, char **begin, list *l);

/* Public Procudures */

/* To do :
 * (1) Deal with escape
 *
 * str is a string looks like "abc\"d", "123" */
char * strPreProcessing(char *str) {
    list *l = listCreate();
    char *current = str, current_c = *str, *begin = NULL;

    while (current_c != '\0') {
        // Deal with newline escape
        if (current_c == BACK_SLASH && *(current + 1) == 'n')
            current = newLineProcessing(current, &begin, l);
        // Remove quotation which without escape
        if (current_c == QUOTA)
            current = quotaProcessing(current, &begin, l);

        current_c = *(++current);
    }

    listIter iter = listGetIter(l, LITER_FORWARD);
    while ((current = listNext_v(&iter)) != NULL) {
        // len = strlen(string) + strlen('\0')
        int len = strlen(current);

        strncpy(begin, current, len + 1);
        begin += len;
    }

    listRelease(l);
    return str;
}

/* Private Procedures */
private char * newLineProcessing(char *str, char **begin, list *l) {
    char *current = str, *next = str + 1;

    *current = '\n';
    *next = '\0';

    if (*begin == NULL) *begin = next;
    listAppend(l, next + 1);

    return next;
}


private char * quotaProcessing(char *str, char **begin, list *l) {
    *str = '\0';

    if (*begin == NULL) *begin = str;
    listAppend(l, str + 1);

    return str;
}

#ifdef _AST_TREE_TESTING_

void miscTest(void **state) {}

#endif /* _AST_TREE_TESTING_ */
