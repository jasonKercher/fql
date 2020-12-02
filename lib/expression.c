#include "query.h"

#include "util/util.h"

Expression* expression_new(enum expr_type type, void* data)
{
        Expression* new_expression = NULL;
        malloc_(new_expression, sizeof(*new_expression));

        return expression_init(new_expression, type, data);
}

Expression* expression_init(Expression* expr, enum expr_type type, void* data)
{
        *expr = (Expression) {
                 type   /* type */
                ,data   /* expression */
        };

        return expr;
}

void expression_free(Expression* expr)
{
        /**
         * Expressions own their data.  Because an expression
         * can contain many different types, how we free expr
         * varies based on type
         */

        switch (expr->type) {
        default:
                free_(expr->data);
        }

        free_(expr);
}
