#include "reader.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <csv.h>

#include "fql.h"
#include "process.h"
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

        vec_reserve(csv_data->csv_records, buflen);

        /* TODO: (libcsv) Would be nice if we could stack
         *       these or put them in contiguous memory.
         */
        int i = 0;
        for (; i < buflen; ++i) {
                csv_record* rec = csv_record_new();
                vec_push_back(csv_data->csv_records, &rec);
        }

        return csv_data;
}

void libcsv_free(void* reader_data)
{
        struct libcsv_data* csv_data = reader_data;
        csv_reader_free(csv_data->csv_handle);

        csv_record** recs = csv_data->csv_records->data;
        for (; recs != vec_end(csv_data->csv_records); ++recs) {
                csv_record_free(*recs);
        }
        vec_free(csv_data->csv_records);
}

int libcsv_get_record(void* reader_data, Vec* rec, unsigned char idx)
{
        struct libcsv_data* csv = reader_data;
        csv_record** csv_rec = vec_at(csv->csv_records, idx);

        if (csv_get_record(csv->csv_handle, *csv_rec) == CSV_FAIL) {
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

struct mmapcsv_data* mmapcsv_new(size_t buflen)
{
        struct mmapcsv_data* new_data = NULL;
        malloc_(new_data, sizeof(*new_data));

        return mmapcsv_init(new_data, buflen);
}

struct mmapcsv_data* mmapcsv_init(struct mmapcsv_data* csv_data, size_t buflen)
{
        *csv_data = (struct mmapcsv_data) {
                 csv_reader_new()       /* csv_handle */
                ,vec_new_(csv_record*)  /* csv_records */
                ,{ NULL, 0 }            /* current */
                ,vec_new_(StringView)   /* raw */
                ,NULL                   /* rec_map */
                ,NULL                   /* map */
                ,0                      /* map_idx */
                ,0                      /* file_size */
                ,0                      /* fd */
        };

        vec_reserve(csv_data->csv_records, buflen);

        int i = 0;
        for (; i < buflen; ++i) {
                csv_record* rec = csv_record_new();
                vec_push_back(csv_data->csv_records, &rec);
        }

        return csv_data;
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

        csv_record** recs = data->csv_records->data;
        for (; recs != vec_end(data->csv_records); ++ recs) {
                csv_record_free(*recs);
        }
        vec_free(data->csv_records);

        if (munmap(data->mmap_base, data->file_size)) {
                perror("munmap");
        }
        close(data->fd);
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
                                    *(++data)->mp == '\n') {
                                        ++data->mp;
                                }
                                vec_push_back(data->raw, &data->current);
                                return FQL_GOOD;
                        default:
                                ++data->mp;
                }
        }
        return FQL_FAIL;  /* EOF?? */
} 

int mmapcsv_get_record(void* reader_data, Vec* rec, unsigned char idx)
{
        struct mmapcsv_data* data = reader_data;
        csv_record** csv_rec = vec_at(data->csv_records, idx);

        if (mmapcsv_getline(data)) {
                return FQL_FAIL;
        }

        /* lol. lets just call everything data */
        csv_nparse(data->csv_handle, *csv_rec, data->current.data, data->current.len);

        vec_resize(rec, (*csv_rec)->size);

        StringView* sv = vec_begin(rec);
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

        return FQL_GOOD;
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
                 READ_UNDEFINED /* type */
                ,NULL           /* reader_data */
                ,NULL           /* get_record_fn */
                ,NULL           /* free_fn */
                ,""             /* file_name */
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

//Vec* reader_get_recs(Reader* reader)
//{
//        switch (reader->type) {
//        case READ_LIBCSV:
//        {
//                struct libcsv_data* data = reader->reader_data;
//                return data->recs;
//        }
//        case READ_MMAPCSV:
//        {
//                struct mmapcsv_data* data = reader->reader_data; 
//                return data->recs;
//        }
//        default:
//                fprintf(stderr, "%d: unknown read_type\n", reader->type);
//        }
//
//        return NULL;
//}

void reader_assign(Reader* reader)
{
        int ret = 0;
        switch (reader->type) {
        case READ_LIBCSV:
        {
                struct libcsv_data* data = libcsv_new(PROCESS_BUFFER_SIZE);
                reader->reader_data = data;
                reader->get_record_fn = &libcsv_get_record;
                reader->free_fn = &libcsv_free;
                ret = csv_reader_open(data->csv_handle, reader->file_name);
                break;
        }
        case READ_MMAPCSV:
        {
                struct mmapcsv_data* data = mmapcsv_new(PROCESS_BUFFER_SIZE);
                reader->reader_data = data;
                reader->get_record_fn = &mmapcsv_get_record;
                reader->free_fn = &mmapcsv_free;
                ret = mmapcsv_open(data, reader->file_name);
                break;
        }
        case READ_SUBQUERY:
                fputs("cannot assign reader for subquery\n", stderr);
                break;
        default:
                fprintf(stderr, "%d: unknown read_type\n", reader->type);
        }

        if (ret == CSV_FAIL) {
                csv_perror_exit();
        }
}

