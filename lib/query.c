#include "query.h"

#include "util/util.h"

query_t* query_new()
{
        query_t* new_query = NULL;
        malloc_(new_query, sizeof(*new_query));

        *new_query = (query_t) {
                 table_new()            /* table */
                ,NULL                   /* sources */
                ,NULL                   /* conditions */
                ,NULL                   /* groups */
                ,NULL                   /* having */
                ,NULL                   /* limit */
                ,OP_NONE                /* operation */

                ,NULL                   /* expr */
                ,0                      /* mode */
        };

        return new_query;
}
