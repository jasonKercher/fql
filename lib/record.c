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
                ,{ 0 }  /* rec_cpy */
                ,{ 0 }  /* raw_rec */
        };

        vec_construct_(&rec->fields, StringView);
        string_construct(&rec->rec_cpy);
        return rec;
}

void record_free(Record* rec)
{
        record_destroy(rec);
        free_(rec);
}

void record_destroy(Record* rec)
{
        string_destroy(&rec->rec_cpy);
        vec_destroy(&rec->fields);
}
