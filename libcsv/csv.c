#include "csverror.h"
#include "misc.h"
#include "csv.h"
#include "util.h"

struct csv_internal {
        char* buffer;
        size_t bufIdx;
        size_t bufferSize;
        int fieldsAllocated;
};

void csv_perror()
{
        err_printall();
}

void csv_perror_exit()
{
        csv_perror();
        exit(EXIT_FAILURE);
}

struct csv_record* csv_record_new()
{
        struct csv_record* new_rec = NULL;
        malloc_(new_rec, sizeof(*new_rec));

        *new_rec = (struct csv_record) {
                NULL,
                NULL,
                0
        };

        malloc_(new_rec->_in, sizeof(*new_rec->_in));
        *new_rec->_in = (struct csv_internal) {
                NULL,
                0,
                0,
                0
        };

        increase_buffer(&new_rec->_in->buffer, &new_rec->_in->bufferSize);

        return new_rec;
}

void csv_record_free(struct csv_record* this)
{
        int i = 0;
        if (this->_in == NULL) { /* Was cloned */
                for (; i < this->size; ++i)
                        free_(this->fields[i]);
        } else {
                free_(this->_in->buffer);
                free_(this->_in);
        }
        free_(this->fields);
        free_(this);
}

void csv_record_free_not_fields(struct csv_record* this)
{
        int i = 0;
        if (this->_in == NULL) { /* Was cloned */
                for (; i < this->size; ++i)
                        free_(this->fields[i]);
        } else {
                free_(this->_in->buffer);
                free_(this->_in);
        }
        //free_(this->fields);
        free_(this);
}

struct csv_record* csv_record_clone(struct csv_record* rec)
{
        struct csv_record* new_rec = NULL;
        malloc_(new_rec, sizeof(*new_rec));

        *new_rec = (struct csv_record) {
                NULL,
                NULL,
                NULL,
                rec->size
        };

        malloc_(new_rec->fields, rec->size * sizeof(char*));

        int i = 0;
        for (; i < rec->size; ++i)
                new_rec->fields[i] = strdup(rec->fields[i]);

        return new_rec;
}
