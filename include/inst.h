/* Inst.h */

#ifndef _INST_H_
#define _INST_H_

#include "type.h"

#define MAX_LENGTH_OF_SCRIPT_IDENT 32

/* Inst basis */
typedef _Bool (*isIdentExistsFunc)(void *request);
/* Apply a transformation to a Inst, a frame will be generated. */
typedef char * (*encodeFunc)(void *request);
/* Apply a transformation to a frame and the content of frame will
 * be copy into the Inst. */
typedef _Status_t (*decodeFunc)(void *request, char *frame);
/* Encapsulate basis of Inst into a struct
 * and different kinds of inst ca have different basis */
typedef struct InstType {
    isIdentExistsFunc isExists;
    encodeFunc        encode;
    decodeFunc        decode;
} InstType;

typedef struct Inst {
    char *identity;
    InstType type;
} Inst;

#endif /* _INST_H_ */

