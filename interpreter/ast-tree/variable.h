/* variable.h */

#ifndef _AST_TREE_VARIABLE_H_
#define _AST_TREE_VARIABLE_H_

#include "object.h"
#include "primitive.h"

typedef enum { VAR_EMPTY, VAR_PRIMITIVE, VAR_OBJECT } VarType;

typedef enum {
    VAR_OP_PLUS,
    VAR_OP_MINUS,
    VAR_OP_MUL,
    VAR_OP_LESS_THAN,
    VAR_OP_GREATER_THAN,
    VAR_OP_LESS_OR_EQUAL,
    VAR_OP_GREATER_OR_EQUAL,
    VAR_OP_NOT_EQUAL
} VarOp;

typedef struct Variable {
    /* ident of variable, this field
     * may will be null if its a te-
     * mporary variable. */
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
#define VAR_IS_PRIMITIVE(V) ((V)->type == VAR_PRIMITIVE)
#define VAR_IS_OBJECT(V)    ((V)->type == VAR_OBJECT)

/* Prototypes */
Variable varDefault_Empty();
Variable * varDefault();
Variable * varGen(char *ident, VarType type, void *value);

// Operators
Variable * varPlusOp(Variable *l, Variable *r);
Variable * varMinusOp(Variable *l, Variable *r);
Variable * varMulOp(Variable *l, Variable *r);
Variable * varDivOp(Variable *l, Variable *r);
Variable * varLessThanOp(Variable *l, Variable *r);
Variable * varGreaterThanOp(Variable *l, Variable *r);
Variable * varLessOrEqualOp(Variable *l, Variable *r);
Variable * varGreaterOrEqualOp(Variable *l, Variable *r);
Variable * varNotEqualOp(Variable *l, Variable *r);

#endif /* _AST_TREE_VARIABLE_H_ */
