/* InstType.c */

#include "inst.h"
#include "type.h"

/* dispatcherInst */
_Bool isIdentExists_dispatcher(void *inst) {

}
char * encode_dispatcher(void *request) {

}
_Status_t decode_dispatcher(void *request, char *frame) {

}

InstType dispatcherInst = {
    isIdentExists_dispatcher,
    encode_dispatcher,
    decode_dispatcher
};

