/* wrapper.c */

#include "wrapper.h"
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include "list.h"

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

/* list<pair<str, rep> > */
char * strReplace(char *src, list *pair_list) {}

/* list<char *> */
list * strSplit(char *src, char spliter, char end) {
    list *l = listCreate();;

    int length = 0;

    char *begin, // The position of first character of a piece
        *tail,   // The position of the first spliter or null after piece
        *dest = NULL;

    begin = src, tail = begin + 1;

    while (*begin != end) {
        begin = tail;
        /* fixme: Is it dangerous ? */
        tail = begin + 1;

        while (*tail != spliter) {
            ++tail;
        }
        /* Spliting */
        length = tail - begin;
        dest = (char *)zMalloc(length);
        strncpy(dest, begin, length);

        listAppend(l, dest);

        begin = tail;
    }

    return l;
}
