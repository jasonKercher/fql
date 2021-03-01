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
                ,fifo_new_(csv_record*, 256)
                ,false
        };

        /* TODO: (libcsv) Would be nice if we could stack
         *       these or put them in contiguous memory.
         */
        Vec* buf = csv_data->csv_recs->buf;
        csv_record** it = vec_begin(buf);
        for (; it != vec_end(buf); ++it) {
                *it = csv_record_new();
        }
        fifo_advance(csv_data->csv_recs);

        return csv_data;
}

void libcsv_reader_free(void* reader_data)
{
        if (reader_data == NULL) {
                return;
        }
        struct libcsv_reader* csv_data = reader_data;
        csv_reader_free(csv_data->csv_handle);

        Vec* buf = csv_data->csv_recs->buf;
        csv_record** it = vec_begin(buf);
        for (; it != vec_end(buf); ++it) {
                csv_record_free(*it);
        }
        fifo_free(csv_data->csv_recs);

        free_(csv_data);
}

char* libcsv_get_delim(struct libcsv_reader* csv)
{
        return csv->csv_handle->delimiter;
}

int libcsv_get_record(Reader* reader, Record* rec)
{
        struct libcsv_reader* csv = reader->reader_data;

        if (csv->eof) {
                return csv_reader_reset(csv->csv_handle);
        }

        csv_record** csv_rec = fifo_peek(csv->csv_recs);
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

        fifo_consume(csv->csv_recs);
        fifo_advance(csv->csv_recs);

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

        string_strncpy(&rec->rec_cpy, (*csv_rec)->raw, (*csv_rec)->raw_len);
        rec->rec_raw.data = (*csv_rec)->raw;
        rec->rec_raw.len = (*csv_rec)->raw_len;

        return FQL_GOOD;
}

int libcsv_reset(Reader* reader)
{
        struct libcsv_reader* csv = reader->reader_data;
        csv->eof = false;
        csv_reader_reset(reader->reader_data);

        /* Skip header */
        csv_record** csv_rec = fifo_peek(csv->csv_recs);
        return csv_get_record_to(csv->csv_handle, *csv_rec, reader->max_col_idx+1);
}
