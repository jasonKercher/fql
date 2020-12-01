#include "query.h"

#include "util/util.h"

Expression* expression_new(enum expr_type type, void* expr_data)
{
        Expression* new_expression = NULL;
        malloc_(new_expression, sizeof(*new_expression));

        return expression_init(new_expression, type, expr_data);
}

Expression* expression_init(Expression* expr, enum expr_type type, void* expr_data)
{
        *expr = (Expression) {
                 type   /* type */
                ,expr   /* expression */
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
                free_(expr->expr);
        }

        free_(expr);
}
