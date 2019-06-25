/* variable.h */

#ifndef _AST_TREE_VARIABLE_H_
#define _AST_TREE_VARIABLE_H_

#include "object.h"
#include "primitive.h"

typedef enum { VAR_EMPTY, VAR_PRIMITIVE, VAR_OBJECT } VarType;

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

/* Member function implement as macros */
#define VAR_SET_PRIMITIVE(V, P) ({ (V)->type = VAR_PRIMITIVE; (V)->p = (P) })
#define VAR_SET_OBJECT(V, O) ({ (V)->type = VAR_OBJECT; (V)->o = (O); })

/* Prototypes */
Variable varDefault_Empty();
Variable * varDefault();
Variable * varGen(char *ident, VarType type, void *value);

#endif /* _AST_TREE_VARIABLE_H_ */
