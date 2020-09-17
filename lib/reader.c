#include "reader.h"

#include <csv.h>

#include "util/util.h"

reader_t* reader_new()
{
        reader_t* new_reader = NULL;
        malloc_(new_reader, sizeof(*new_reader));

        *new_reader = (reader_t) {
                 NULL   /* handle */
                ,NULL   /* get_record_f */
                ,NULL   /* free_f */
                ,""     /* file_name */
        };

        return new_reader;
}

void reader_free(reader_t* reader)
{
        if (reader->free_f) {
                reader->free_f(reader->handle);
        }
        free_(reader);
}

/** Interfaces for libcsv **/
int csv_get_record_i(void* handle, csv_record* rec);
void csv_reader_free_i(void* handle);

void reader_assign(reader_t* reader, enum read_type type)
{
        int ret = 0;
        switch (type) {
        case READ_LIBCSV:
                reader->handle = csv_reader_new();
                reader->get_record_f = &csv_get_record_i;
                reader->free_f = &csv_reader_free_i;
                ret = csv_reader_open(reader->handle, reader->file_name);
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

int csv_get_record_i(void* handle, csv_record* rec)
{
        return csv_get_record((csv_reader*) handle, rec);       
}

void csv_reader_free_i(void* handle)
{
        csv_reader_free((csv_reader*) handle);
}

