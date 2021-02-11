#include "record.h"
#include "util/util.h"
#include "util/stringview.h"

Record* record_new()
{
        Record* new_record = NULL;
        malloc_(new_record, sizeof(*new_record));

        return record_construct(new_record);
}

Record* record_construct(Record* rec)
{
        *rec = (Record) {
                 { 0 }  /* fields */
                ,{ 0 }  /* extra */
        };

        vec_construct_(&rec->fields, StringView);
        return rec;
}

void record_free(Record* rec)
{
        record_destroy(rec);
        free_(rec);
}

void record_destroy(Record* rec)
{
        vec_destroy(&rec->fields);
}
