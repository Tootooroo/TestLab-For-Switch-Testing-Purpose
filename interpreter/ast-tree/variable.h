/* variable.h */

#ifndef _AST_TREE_VARIABLE_H_
#define _AST_TREE_VARIABLE_H_

typedef enum { VAR_PRIMITIVE, VAR_OBJECT } VarType;

#include "object.h"
#include "primitive.h"

typedef struct Variable {
    /* ident of variable, this field
     * may will be  null if the var is
     * return by procedure */
    char *identifier;

    // Type of variable
    VarType type;

    union {
        Primitive *p;
        Object *o;
    };
} Variable;

#endif /* _AST_TREE_VARIABLE_H_ */
