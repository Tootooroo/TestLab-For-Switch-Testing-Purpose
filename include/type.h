/* type.h */

#ifndef _TEST_LAB_TYPE_
#define _TEST_LAB_TYPE_

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#ifndef null
#define null 0
#endif

#ifndef ok
#define ok 0
#endif

#ifndef error
#define error 1
#endif

typedef int _Status_t;
typedef unsigned int _Index_t;

#define containerOf(addr, contType, member) \
    do {\
        typeof( ((contType)0)->member) *__ptr = addr;\
        (contType *)((char *)__ptr - offsetof(contType, member));\
    } while(0);
#define isNull(obj) (obj == null)

#endif /* _TEST_LAB_TYPE_ */

