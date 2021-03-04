#include "record.h"
#include "util/vec.h"
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
                 vec_new_(StringView)   /* fields */
                ,csv_record_new()       /* libcsv_rec */
                ,string_new()           /* rec_cpy */
                ,{ 0 }                  /* raw_rec */
                ,true                   /* consumable */
        };

        return rec;
}

void record_free(Record* rec)
{
        record_destroy(rec);
        free_(rec);
}

void record_destroy(Record* rec)
{
        vec_free(rec->fields);
        csv_record_free(rec->libcsv_rec);
        string_free(rec->rec_cpy);
}
