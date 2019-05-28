/* wrapper.c */

#include "wrapper.h"
#include <malloc.h>
#include <string.h>
#include <stdio.h>

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

