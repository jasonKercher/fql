#include "query.h"

#include "util/util.h"

expression_t* expression_new()
{
        expression_t* new_expression = NULL;
        malloc_(new_expression, sizeof(*new_expression));

        *new_expression = (expression_t) {
                 NULL           /* output_table */
                ,EXPR_NONE      /* type */
        };

        return new_expression;
}


