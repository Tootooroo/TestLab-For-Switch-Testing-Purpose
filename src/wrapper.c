/* wrapper.c */

#include "wrapper.h"
#include <malloc.h>
#include <string.h>
#include <stdio.h>

/* Return an initialized memory buffer */
void * zMalloc(size_t size) {
    if (size <= 0) return null;

    void *mem = malloc(size);
    if (isNull(mem)) {
        printf("Malloc error\n");
        exit(1);
    }
    memset(mem, 0, size);

    return mem;
}

/* Transfer number string into integer
 *
 * fixme: inpute string format should be checked via regex */
int str2Num(char *str) {
    int num;

    sscanf(str, "%d", &num);

    return num;
}

_Bool strCompare(char *left, char *right) {
    size_t len_left, len_right;

    if (isNull(left) || isNull(right)) return false;

    len_left = strlen(left);
    len_right = strlen(right);

    if (len_left != len_right)
        return false;

    return !strncmp(left, right, len_left);
}
