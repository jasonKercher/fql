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
                ,vec_new_(csv_record*)  /* csv_recs */
                ,{ NULL, 0 }            /* current */
                ,vec_new_(StringView)   /* raw */
                ,NULL                   /* rec_map */
                ,NULL                   /* mmap_base */
                ,0                      /* mp */
                ,0                      /* file_size */
                ,0                      /* fd */
        };

        vec_reserve(csv_data->csv_recs, buflen);

        int i = 0;
        for (; i < buflen; ++i) {
                csv_record* rec = csv_record_new();
                vec_push_back(csv_data->csv_recs, &rec);
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

        csv_record** recs = data->csv_recs->data;
        for (; recs != vec_end(data->csv_recs); ++ recs) {
                csv_record_free(*recs);
        }
        vec_free(data->csv_recs);

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
                            *(++data->mp) == '\n') {
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
        csv_record** csv_rec = vec_at(data->csv_recs, idx);

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

void mmapcsv_reset(void* reader_data)
{
        struct mmapcsv_data* data = reader_data;
        data->mp = data->mmap_base;
}
