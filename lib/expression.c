#include "query.h"

#include "util/util.h"

struct expression* expression_new(enum expr_e type, void* expr)
{
        struct expression* new_expression = NULL;
        malloc_(new_expression, sizeof(*new_expression));

        *new_expression = (struct expression) {
                 expr   /* expression */
                ,type   /* type */
        };

        return new_expression;
}

void expression_free(struct expression* expr)
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
