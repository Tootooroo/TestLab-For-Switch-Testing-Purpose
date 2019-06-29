/* pair.c */

#include "pair.h"
#include "wrapper.h"

/* Public procedures */
Pair * pairDefault(void *dupMethod, void *releaseMethod) {
    Pair *p = (Pair *)zMalloc(sizeof(Pair));
    p->pairDup = dupMethod;
    p->pairRelease = releaseMethod;
    return p;
}

Pair * pairGen(void *left, void *right, void *dupMethod, void *releaseMethod) {
    Pair *p = pairDefault(dupMethod, releaseMethod);

    PAIR_SET_LEFT(p, left);
    PAIR_SET_RIGHT(p, right);

    return p;
}

Pair * pairGen_Integer(int left, int right, void *dupMethod, void *releaseMethod) {
    Pair *p = pairDefault(dupMethod, releaseMethod);

    PAIR_SET_LEFT_I(p, left);
    PAIR_SET_RIGHT_I(p, right);

    return p;
}
