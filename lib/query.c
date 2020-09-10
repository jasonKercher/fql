#include "query.h"

#include "util/util.h"

query_t* query_new()
{
        query_t* new_query = NULL;
        malloc_(new_query, sizeof(*new_query));

        *new_query = (query_t) {
                 operation_new()   /* operation */
                ,NULL              /* sources */
                ,NULL              /* conditions */
                ,NULL              /* groups */
                ,NULL              /* having */
                ,NULL              /* limit */
        };

        return new_query;
}
