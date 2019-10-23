/* file: methods.c */

#include "type.h"
#include "wrapper.h"

/****************
 * List Methods *
 ****************/

/* Methods for pair<_, _> */
#include "pair.h"

_Status_t lOps_pair_release(void *v) {
    pair *p = v;
    p->pairRelease(p);

    return OK;
}

/* vl: pair<String, _>
 * vr: String */
_Bool lOps_pair_match_key(const void *vl, const void *vr) {
    pair *pl = (pair *)vl;
    return strCompare(PAIR_GET_LEFT(pl), (char *)vr);
}

_Bool lOps_pair_match(const void *vl, const void *vr) {
    pair *pl = vl, *pr = vr;
    return pl->pairCmp(pl, pr);
}

/****************
 * Pair Methods *
 ****************/
