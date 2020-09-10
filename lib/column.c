#include "query.h"

#include "util/util.h"

column_t* column_new()
{
        column_t* new_column = NULL;
        malloc_(new_column, sizeof(*new_column));

        *new_column = (column_t) {
                 NULL   /* table */
                ,""     /* name */
                ,""     /* alias */
                ,0      /* location */
                ,0      /* width */
        };

        return new_column;
}


