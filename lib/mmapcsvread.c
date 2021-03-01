#include "reader.h"
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <csv.h>
#include "fql.h"


struct mmapcsv_data* mmapcsv_new(size_t buflen)
{
        struct mmapcsv_data* new_data = NULL;
        malloc_(new_data, sizeof(*new_data));

        return mmapcsv_construct(new_data, buflen);
}

struct mmapcsv_data* mmapcsv_construct(struct mmapcsv_data* csv_data, size_t buflen)
{
        *csv_data = (struct mmapcsv_data) {
                 csv_reader_new()       /* csv_handle */
                ,fifo_new_(csv_record*, 256)
                ,{ NULL, 0 }            /* current */
                ,vec_new_(StringView)   /* raw */
                ,NULL                   /* rec_map */
                ,NULL                   /* mmap_base */
                ,0                      /* mp */
                ,0                      /* file_size */
                ,0                      /* fd */
                ,false                  /* eof */
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

char* mmapcsv_get_delim(struct mmapcsv_data* data)
{
        return data->csv_handle->delimiter;
}

int mmapcsv_open(struct mmapcsv_data* data, const char* file_name)
{
        data->fd = open(file_name, O_RDONLY);
        if (data->fd == -1) {
                perror(file_name);
                return FQL_FAIL;
        }

        struct stat sb;
        if (fstat(data->fd, &sb) == -1) {
                perror(file_name);
                return FQL_FAIL;
        }

        data->file_size = sb.st_size;

        data->mmap_base = mmap(NULL, data->file_size, PROT_READ, MAP_PRIVATE, data->fd, 0);
        if (data->mmap_base == MAP_FAILED) {
                perror("file_name");
                return FQL_FAIL;
        }

        madvise(data->mmap_base, data->file_size, MADV_SEQUENTIAL);
        data->mp = data->mmap_base;
        return FQL_GOOD;
}

void mmapcsv_free(void* reader_data)
{
        struct mmapcsv_data* data = reader_data;
        csv_reader_free(data->csv_handle);

        Vec* buf = data->csv_recs->buf;
        csv_record** it = vec_begin(buf);
        for (; it != vec_end(buf); ++it) {
                csv_record_free(*it);
        }
        fifo_free(data->csv_recs);

        if (munmap(data->mmap_base, data->file_size)) {
                perror("munmap");
        }
        close(data->fd);
        free_(data);
}

int mmapcsv_getline(struct mmapcsv_data* data)
{
        /* string view will point to the record in the raw data */
        data->current.data = data->mp;

        while (data->mp - data->mmap_base < data->file_size) {
                switch (*data->mp) {
                case '\n':
                        data->current.len = data->mp - data->current.data;
                        ++data->mp;
                        vec_push_back(data->raw, &data->current);
                        return FQL_GOOD;
                case '\r':
                        data->current.len = data->mp - data->current.data;
                        if (data->mp - data->mmap_base + 1 < data->file_size &&
                            *(++data->mp) == '\n') {
                                ++data->mp;
                        }
                        vec_push_back(data->raw, &data->current);
                        return FQL_GOOD;
                default:
                        ++data->mp;
                }
        }

        data->eof = true;
        return EOF;
}

int mmapcsv_get_record(Reader* reader, Record* rec)
{
        struct mmapcsv_data* data = reader->reader_data;
        if (data->eof) {
                return mmapcsv_reset(reader);
        }

        csv_record** csv_rec = fifo_peek(data->csv_recs);
        int ret = mmapcsv_getline(data);
        if (ret) {
                return ret;
        }

        /* lol. lets just call everything data */
        ret = csv_nparse_to(data->csv_handle, *csv_rec, data->current.data, data->current.len, reader->max_col_idx+1);
        switch (ret) {
        case CSV_GOOD:
                break;
        case CSV_FAIL:
                return FQL_FAIL;
        }

        fifo_consume(data->csv_recs);
        fifo_advance(data->csv_recs);

        /* This should really never change unless we
         * want this to mean something (like NULLs).
         */
        vec_resize(&rec->fields, (*csv_rec)->size);

        StringView* sv = vec_begin(&rec->fields);
        char** fields = (*csv_rec)->fields;

        int i = 0;
        for (; i < (*csv_rec)->size; ++i) {
                sv[i].data = fields[i];
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

int mmapcsv_get_record_at(Reader* reader, Record* rec, char* location)
{
        struct mmapcsv_data* data = reader->reader_data;
        data->eof = false;
        data->mp = location;
        return mmapcsv_get_record(reader, rec);
}

int mmapcsv_reset(Reader* reader)
{
        struct mmapcsv_data* data = reader->reader_data;
        data->eof = false;
        data->mp = data->mmap_base;
        /* Skip header */
        return mmapcsv_getline(data);
}
