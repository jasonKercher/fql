#include "query.h"

#include "util/util.h"

expression_t* expression_new(int type, void* expr)
{
        expression_t* new_expression = NULL;
        malloc_(new_expression, sizeof(*new_expression));

        *new_expression = (expression_t) {
                 expr           /* expression */
                ,EXPR_NONE      /* type */
        };

        return new_expression;
}


