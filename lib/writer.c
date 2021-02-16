#include "writer.h"
#include <csv.h>
#include "fql.h"
#include "column.h"
#include "util/stringview.h"

//struct libcsv_writer* libcsv_writer_new()
//{
//        struct libcsv_writer* new_data = NULL;
//        malloc_(new_data, sizeof(*new_data));
//
//        return libcsv_writer_construct(new_data);
//}
//
//struct libcsv_writer* libcsv_writer_construct(struct libcsv_writer* writer)
//{
//        *writer = (struct libcsv_writer) {
//                 csv_writer_new()       /* csv_handle */
//        };
//        return writer;
//}
//
void libcsv_writer_free(void* writer_data)
{
        csv_writer_free(writer_data);
}

int libcsv_write_record(void* writer_data, Vec* col_vec, Vec* recs)
{
        csv_writer* handle = writer_data;

        Column** cols = vec_begin(col_vec);
        int i = 0;
        for (; i < col_vec->size; ++i) {
                StringView sv;
                if (i > 0) {
                        fputs(handle->delimiter, handle->file);
                }

                if (cols[i]->expr == EXPR_ASTERISK) {
                        int quote_store = handle->quotes;
                        handle->quotes = QUOTE_NONE;
                        Record** rec = vec_at(recs, cols[i]->src_idx);
                        csv_nwrite_field(handle, (*rec)->raw.data, (*rec)->raw.size);
                        handle->quotes = quote_store;
                }
                else {
                        if (column_get_stringview(&sv, cols[i], recs)) {
                                return FQL_FAIL;
                        }
                        csv_nwrite_field(handle, sv.data, sv.len);
                }
        }

        fputs(handle->line_terminator, handle->file);

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
                ,{ 0 }                  /* file_name */
        };

        string_construct(&writer->file_name);

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
        string_destroy(&writer->file_name);
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
                csv_writer* data = csv_writer_new();
                writer->writer_data = data;
                writer->write_record_fn = &libcsv_write_record;
                writer->free_fn = &libcsv_writer_free;
                break;
        }
        default:
                fprintf(stderr, "%d: unknown write_type\n", writer->type);
        }
}
