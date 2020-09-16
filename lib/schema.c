#include "schema.h"
#include "column.h"
#include "util/util.h"

schema_t* schema_new()
{
        schema_t* new_schema = NULL;
        malloc_(new_schema, sizeof(*new_schema));

        *new_schema = (schema_t) {
                 NULL    /* columns */
                ,""      /* name */
        };

        return new_schema;
}

void schema_free(void* generic_schema)
{
        schema_t* schema = generic_schema;
        stack_free_func(&schema->columns, &column_free);
        free_(schema);
}

void schema_resolve(queue_t* query_list)
{

}
