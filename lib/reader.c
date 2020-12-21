#include "reader.h"
#include "fql.h"
#include "process.h"

#include <csv.h>

#include "util/stringview.h"
#include "util/util.h"

struct libcsv_data* libcsv_new(size_t buflen)
{
        struct libcsv_data* new_data = NULL;
        malloc_(new_data, sizeof(*new_data));

        return libcsv_init(new_data, buflen);
}

struct libcsv_data* libcsv_init(struct libcsv_data* csv_data, size_t buflen)
{
        *csv_data = (struct libcsv_data) {
                 csv_reader_new()
                ,vec_new_(csv_record*)
        };

        vec_reserve(csv_data->records, buflen);

        /* TODO: (libcsv) Would be nice if we could stack
         *       these or put them in contiguous memory.
         */
        int i = 0;
        for (; i < buflen; ++i) {
                csv_record* rec = csv_record_new();
                vec_push_back(csv_data->records, &rec);
        }

        return csv_data;
}

int libcsv_get_record(void* reader_data, Vec* rec, unsigned char idx)
{
        struct libcsv_data* csv = reader_data;
        csv_record** csv_rec = vec_at(csv->records, idx);

        if (csv_get_record(csv->handle, *csv_rec) == CSV_FAIL) {
                return FQL_FAIL;
        }

        vec_resize(rec, (*csv_rec)->size);

        StringView* sv = vec_begin(rec);
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

        return FQL_GOOD;
}

void libcsv_free(void* reader_data)
{
        struct libcsv_data* csv_data = reader_data;
        csv_reader_free(csv_data->handle);

        csv_record** recs = csv_data->records->data;
        for (; recs != vec_end(csv_data->records); ++ recs) {
                csv_record_free(*recs);
        }
        vec_free(csv_data->records);
}

Reader* reader_new()
{
        Reader* new_reader = NULL;
        malloc_(new_reader, sizeof(*new_reader));

        return reader_init(new_reader);
}

Reader* reader_init(Reader* reader)
{
        *reader = (Reader) {
                 NULL   /* reader_data */
                ,NULL   /* get_record_fn */
                ,NULL   /* free_fn */
                ,""     /* file_name */
        };

        return reader;
}

void reader_free(Reader* reader)
{
        if (reader == NULL) {
                return;
        }
        if (reader->free_fn) {
                reader->free_fn(reader->reader_data);
        }
        free_(reader);
}

void reader_assign(Reader* reader, enum read_type type)
{
        int ret = 0;
        switch (type) {
        case READ_LIBCSV:
                reader->reader_data = libcsv_new(PROCESS_BUFFER_SIZE);
                reader->get_record_fn = &libcsv_get_record;
                reader->free_fn = &libcsv_free;
                struct libcsv_data* data = reader->reader_data;
                ret = csv_reader_open(data->handle, reader->file_name);
                break;
        case READ_LIBCSV_MMAP:
                //reader->reader_data = libcsv_new(PROCESS_BUFFER_SIZE);
                //reader->get_record_fn = &libcsv_get_record;
                //reader->free_fn = &libcsv_free;
                //ret = csv_reader_open(reader->reader_data, reader->file_name);
                break;
        case READ_SUBQUERY:
                fputs("cannot assign reader for subquery\n", stderr);
                break;
        default:
                fprintf(stderr, "%d: unknown read_type\n", type);
        }

        if (ret == CSV_FAIL) {
                csv_perror_exit();
        }
}

