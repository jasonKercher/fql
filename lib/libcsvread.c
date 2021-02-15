#include "reader.h"
#include "csv.h"
#include "fql.h"

struct libcsv_reader* libcsv_reader_new(size_t buflen)
{
        struct libcsv_reader* new_data = NULL;
        malloc_(new_data, sizeof(*new_data));

        return libcsv_reader_construct(new_data, buflen);
}

struct libcsv_reader* libcsv_reader_construct(struct libcsv_reader* csv_data, size_t buflen)
{
        *csv_data = (struct libcsv_reader) {
                 csv_reader_new()
                ,vec_new_(csv_record*)
                ,false
        };

        vec_reserve(csv_data->csv_recs, buflen);

        /* TODO: (libcsv) Would be nice if we could stack
         *       these or put them in contiguous memory.
         */
        int i = 0;
        for (; i < buflen; ++i) {
                csv_record* rec = csv_record_new();
                vec_push_back(csv_data->csv_recs, &rec);
        }

        return csv_data;
}

void libcsv_reader_free(void* reader_data)
{
        if (reader_data == NULL) {
                return;
        }
        struct libcsv_reader* csv_data = reader_data;
        csv_reader_free(csv_data->csv_handle);

        csv_record** recs = csv_data->csv_recs->data;
        for (; recs != vec_end(csv_data->csv_recs); ++recs) {
                csv_record_free(*recs);
        }
        vec_free(csv_data->csv_recs);

        free_(csv_data);
}

int libcsv_get_record(Reader* reader, Record* rec, unsigned char idx)
{
        struct libcsv_reader* csv = reader->reader_data;
        if (csv->eof) {
                csv_reader_reset(csv->csv_handle);
        }

        csv_record** csv_rec = vec_at(csv->csv_recs, idx);

        int ret = csv_get_record_to(csv->csv_handle, *csv_rec, reader->max_col_idx+1);
        switch (ret) {
        case CSV_GOOD:
                break;
        case CSV_FAIL:
                return FQL_FAIL;
        default:
                csv->eof = true;
                return EOF;
        }

        /* This should really never change unless we
         * want this to mean something (like NULLs).
         */
        vec_resize(&rec->fields, (*csv_rec)->size);

        StringView* sv = vec_begin(&rec->fields);
        char** fields = (*csv_rec)->fields;

        int i = 0;
        for (; i < (*csv_rec)->size; ++i) {
                sv[i].data = fields[i];
                /* TODO: (libcsv) would be nice if it created a trailing
                 *       pointer that pointed at the ending NULL
                 *       terminator. Then, this if block can go.
                 */
                if (i != (*csv_rec)->size - 1) {
                        sv[i].len = fields[i+1] - fields[i] - 1;
                } else {
                        sv[i].len = strlen(fields[i]);
                }
        }

        rec->raw.data = (*csv_rec)->raw;
        rec->raw.len = (*csv_rec)->raw_len;

        return FQL_GOOD;
}

void libcsv_reset(void* reader_data)
{
        struct libcsv_reader* csv = reader_data;
        csv->eof = false;
        csv_reader_reset(reader_data);
}
