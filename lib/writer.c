#include "writer.h"
#include <csv.h>
#include "fql.h"
#include "util/stringview.h"

struct libcsv_writer* libcsv_writer_new()
{
        struct libcsv_writer* new_data = NULL;
        malloc_(new_data, sizeof(*new_data));

        return libcsv_writer_construct(new_data);
}

struct libcsv_writer* libcsv_writer_construct(struct libcsv_writer* writer)
{
        *writer = (struct libcsv_writer) {
                 csv_writer_new()       /* csv_handle */
                ,csv_record_new()       /* csv_rec */
                ,vec_new_(char*)        /* fields */
        };
        return writer;
}

void libcsv_writer_free(void* writer_data)
{
        if (writer_data == NULL) {
                return;
        }

        struct libcsv_writer* data = writer_data;
        csv_writer_free(data->csv_handle);
        csv_record_free_not_fields(data->csv_rec);
        vec_free(data->fields);
        free_(data);
}

int libcsv_write_record(void* writer_data, Vec* string_rec)
{
        struct libcsv_writer* writer = writer_data;

        vec_resize(writer->fields, string_rec->size);

        String* s = vec_begin(string_rec);
        int i = 0;
        for (; i < string_rec->size; ++i) {
                vec_set(writer->fields, i, &s[i].data);
        }

        writer->csv_rec->fields = writer->fields->data;
        writer->csv_rec->size = string_rec->size;

        csv_write_record(writer->csv_handle, writer->csv_rec);

        return FQL_GOOD;
}

Writer* writer_new()
{
        Writer* new_writer = NULL;
        malloc_(new_writer, sizeof(*new_writer));

        return writer_construct(new_writer);
}

Writer* writer_construct(Writer* writer)
{
        *writer = (Writer) {
                 WRITE_UNDEFINED        /* type */
                ,NULL                   /* writer_data */
                ,NULL                   /* write_record_fn */
                ,NULL                   /* free_fn */
                ,vec_new_(String)       /* raw_rec */
                ,""                     /* file_name */
        };

        /* TODO: This should not be here. This should
         *       be dependant on output schema
         */
        writer->type = WRITE_LIBCSV;
        writer_assign(writer);

       return writer;
}

void writer_free(Writer* writer)
{
        if (writer == NULL) {
                return;
        }
        if (writer->free_fn) {
                writer->free_fn(writer->writer_data);
        }
        String* s = vec_begin(writer->raw_rec);
        for (; s != vec_end(writer->raw_rec); ++s) {
                string_destroy(s);
        }
        vec_free(writer->raw_rec);
        free_(writer);
}

void writer_assign(Writer* writer)
{
        int ret = 0;
        switch(writer->type) {
        case WRITE_LIBCSV:
        {
                struct libcsv_writer* data = libcsv_writer_new();
                writer->writer_data = data;
                writer->write_record_fn = &libcsv_write_record;
                writer->free_fn = &libcsv_writer_free;
                break;
        }
        default:
                fprintf(stderr, "%d: unknown write_type\n", writer->type);
        }
}
