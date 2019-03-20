/* type.h */

#ifndef _TEST_LAB_TYPE_
#define _TEST_LAB_TYPE_

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#ifndef null
#define null NULL
#endif

#ifndef ok
#define OK (0)
#endif

#ifndef ERROR
#define ERROR (1)
#endif

#ifndef private
#define private static
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

