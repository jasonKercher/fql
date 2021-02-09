#include "csv.h"
#include <libgen.h>
#include "csvsignal.h"
#include "csverror.h"
#include "charnode.h"
#include "misc.h"
#include "util.h"

/**
 * Internal Structure
 */
struct csv_write_internal {
        FILE* file;
        char* buffer;
        struct charnode* tmp_node;
        char tempname[PATH_MAX];
        char filename[PATH_MAX];
        char filename_org[PATH_MAX];

        size_t bufferSize;
        uint delimLen;
        int recordLen;
};


struct csv_writer* csv_writer_new()
{
        init_sig();

        struct csv_writer* this = NULL;

        malloc_(this, sizeof(*this));
        *this = (struct csv_writer) {
                NULL
                ,","
                ,"\n"
                ,QUOTE_RFC4180
        };

        malloc_(this->_in, sizeof(*this->_in));
        *this->_in = (struct csv_write_internal) {
                stdout  /* file */
                ,NULL   /* buffer */
                ,NULL   /* tmp_node */
                ,""     /* tempname */
                ,""     /* filename */
                ,""     /* filename_org */
                ,0      /* bufferSize */
                ,1      /* delimLen */
                ,0      /* recordLen */
        };

        increase_buffer(&this->_in->buffer, &this->_in->bufferSize);

        return this;

}

void csv_writer_free(struct csv_writer* this)
{
        free_(this->_in->buffer);
        free_(this->_in);
        free_(this);
}

void csv_write_record(struct csv_writer* this, struct csv_record* rec)
{
        int i = 0;
        uint writeIndex = 0;
        int quoteCurrentField = 0;
        uint delimIdx = 0;
        char* c = NULL;

        for (i = 0; i < rec->size; ++i) {
                if (i)
                        fputs(this->delimiter, this->_in->file);

                quoteCurrentField = (this->quotes == QUOTE_ALL);
                writeIndex = 0;
                for (c = rec->fields[i]; *c; ++c) {
                        if (writeIndex + 3 > this->_in->bufferSize)
                                increase_buffer(&this->_in->buffer, &this->_in->bufferSize);
                        this->_in->buffer[writeIndex++] = *c;
                        if (*c == '"' && this->quotes >= QUOTE_RFC4180) {
                                this->_in->buffer[writeIndex++] = '"';
                                quoteCurrentField = true;
                        }
                        if (this->quotes && !quoteCurrentField) {
                                if (strhaschar("\"\n\r", *c))
                                        quoteCurrentField = 1;
                                else if (*c == this->delimiter[delimIdx])
                                        ++delimIdx;
                                else
                                        delimIdx = (*c == this->delimiter[0]) ? 1 : 0;

                                if (delimIdx == this->_in->delimLen)
                                        quoteCurrentField = true;
                        }
                }
                this->_in->buffer[writeIndex] = '\0';
                if (quoteCurrentField)
                        fprintf(this->_in->file, "\"%s\"", this->_in->buffer);
                else
                        fputs(this->_in->buffer, this->_in->file);
        }

        if (rec->extra) {
                if (i)
                        fputs(this->delimiter, this->_in->file);
                for (i = 0; i < rec->extra_len; ++i) {
                        fputc(rec->extra[i], this->_in->file);
                }
        }
        fputs(this->line_terminator, this->_in->file);
}

int csv_writer_reset(struct csv_writer* this)
{
        FAIL_IF(this->_in->file == stdout, "Cannot reset stdout");
        FAIL_IF(!this->_in->file, "No file to reset");
        FAIL_IF(fclose(this->_in->file) == EOF, this->_in->tempname);
        //this->_in->file = NULL;
        this->_in->file = fopen(this->_in->tempname, "w");
        FAIL_IF(!this->_in->file, this->_in->tempname);

        return 0;
}

int csv_writer_mktmp(struct csv_writer* this)
{
        char filename_cp[PATH_MAX] = "";
        strncpy_(filename_cp, this->_in->filename, PATH_MAX);

        char* targetdir = dirname(filename_cp);
        strncpy_(this->_in->tempname, targetdir, PATH_MAX);
        strcat(this->_in->tempname, "/csv_XXXXXX");

        int fd = mkstemp(this->_in->tempname);
        this->_in->file = fdopen(fd, "w");
        FAIL_IF(!this->_in->file, this->_in->tempname);

        this->_in->tmp_node = tmp_push(this->_in->tempname);

        return 0;
}

int csv_writer_isopen(struct csv_writer* this)
{
        if (this->_in->file && this->_in->file != stdout)
                return true;
        return false;
}

int csv_writer_open(struct csv_writer* this, const char* filename)
{
        int ret = 0;
        if(!csv_writer_isopen(this))
                ret = csv_writer_mktmp(this);
        strncpy_(this->_in->filename, filename, PATH_MAX);
        return ret;
}

int csv_writer_close(struct csv_writer* this)
{
        if (this->_in->file == stdout)
                return 0;

        FAIL_IF(fclose(this->_in->file) == EOF, this->_in->tempname);
        this->_in->file = NULL;

        if (this->_in->filename[0] != '\0') {
                int ret = rename(this->_in->tempname, this->_in->filename);
                FAIL_IF(ret, this->_in->tempname);
                ret = chmod(this->_in->filename, 0666);
                FAIL_IF(ret, this->_in->filename);
                tmp_remove_node(this->_in->tmp_node);
        } else {
                FILE* dumpFile = fopen(this->_in->tempname, "r");
                FAIL_IF(!dumpFile, this->_in->tempname);

                char c = '\0';
                while ((c = getc(dumpFile)) != EOF)
                        putchar(c);

                FAIL_IF(fclose(dumpFile) == EOF, this->_in->tempname);
                tmp_remove_file(this->_in->tmp_node->data);
                tmp_remove_node(this->_in->tmp_node);
        }

        return 0;
}

