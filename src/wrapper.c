/* wrapper.c */

#include "wrapper.h"
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include "list.h"
#include "pair.h"

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

char * num2Str(int num) {
    char *str = (char *)zMalloc(numlen(num));

    sprintf(str, "%d", num);
    return str;
}

int numlen(int num) {
    int i = 0;

    do {
        ++i;
    } while (num = num / 10);

    return i;
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


char * charReplace(char *src, char *c, list *pair_string) {
    int len = strlen(src);

    listIter iter = listGetIter(pair_string, LITER_FORWARD);

    char *string;
    while ((string = listNext_v(&iter)) != NULL) {
        len += strlen(string);
    }

    listRewind(&iter);

    int len_scan = 0;
    char *replaced_current, *replaced,
        *scan = src, *head = src, c_char, c_flag = *c;

    replaced_current = replaced = (char *)zMalloc(len);

    while (true) {
        c_char = *scan++;

        if (c_char == c_flag || c_char == '\0') {
            strncpy(replaced_current, head, len_scan);
            head += len_scan + 1;
            replaced_current += len_scan;

            if (c_char == '\0') {
                break;
            }

            char *theStr = (char *)listNext_v(&iter);
            strncpy(replaced_current, theStr, strlen(theStr));
            replaced_current += strlen(theStr);

            len_scan = 0;
            continue;
        }
        ++len_scan;
    }

    return replaced;
}

/* list<char *> */
list * strSplit(char *src, char spliter, char end) {
    list *l = listCreate();;

    char *begin, // The position of first character of a piece
        *tail;   // The position of the first spliter or null after piece

    begin = src, tail = begin + 1;

    while (*begin != end) {
        begin = tail;
        /* fixme: Is it dangerous ? */
        tail = begin + 1;

        while (*tail != spliter) {
            ++tail;
        }
        /* Spliting */
        int length = tail - begin;
        char *dest = (char *)zMalloc(length);
        strncpy(dest, begin, length);

        listAppend(l, dest);

        begin = tail;
    }

    return l;
}


#ifdef _TEST_LAB_UNIT_TESTING_

#include "test.h"

void wrapper_test(void **state) {
    char *str = "12345%12345%12345%";
    char *str1 = "%%%";

    list *l = listCreate();
    listAppend(l, "abc");
    listAppend(l, "abc");
    listAppend(l, "abc");

    assert_int_equal(true, strCompare("abcabcabc", charReplace(str1, "%", l)));
    assert_int_equal(true, strCompare("12345abc12345abc12345abc", charReplace(str, "%", l)));
}

#endif /* _TEST_LAB_UNIT_TESTING_ */
