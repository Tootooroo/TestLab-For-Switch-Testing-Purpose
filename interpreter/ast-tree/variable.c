/* variable.c */

#include "variable.h"

Variable varDefault_Empty() {
    Variable var = {
        .identifier = null,
        .type = VAR_EMPTY,
        .o = null;
    };

    return var;
}

Variable * varDefault() {
    Variable *var = (Variable *)zMalloc(sizeof(Variable));
    *var = varDefault_Empty();

    return var;
}

Variable * varGen(char *ident, VarType type, void *value) {
    Variable *var = varDefault();
    var->identifier = ident;
    var->type = type;

    if (type == VAR_PRIMITIVE) {
        var->p = (Primitive *)value;
    } else if (type == VAR_OBJECT) {
        var->o = (Object *)value;
    }
    return var;
}
