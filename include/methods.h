/* file: methods.h */

#include "type.h"

/****************
 * List Methods *
 ****************/

/* Methods for pair<_, _> */
_Status_t lOps_pair_release(void *);

/* vl: pair<String, _>
 * vr: String */
_Bool lOps_pair_match_key(const void *vl, const void *vr);

_Bool lOps_pair_match(const void *vl, const void *vr);

/****************
 * Pair Methods *
 ****************/
