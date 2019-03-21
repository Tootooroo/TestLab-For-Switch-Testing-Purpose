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

enum {
    Done = 2
};

typedef int _Status_t;
typedef unsigned int _Index_t;

#define isNull(v) ((v) == null)
#define isNonNull(v) ((v) != null)

#endif /* _TEST_LAB_TYPE_ */

