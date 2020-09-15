#include "query.h"

#include "util/util.h"

expression_t* expression_new(int type, void* expr)
{
        expression_t* new_expression = NULL;
        malloc_(new_expression, sizeof(*new_expression));

        *new_expression = (expression_t) {
                 expr   /* expression */
                ,type   /* type */
        };

        return new_expression;
}

void expression_free(expression_t* expr)
{
        /**
         * Expressions own their data.  Because an expression
         * can contain many different types, how we free expr
         * varies based on type
         */

        switch (expr->type) {
        default:
                free_(expr->expr);
        }

        free_(expr);
}
