#include "csv.h"
#include "csvsignal.h"
#include "csverror.h"
#include "safegetline.h"
#include "misc.h"
#include "util.h"

/**
 * Internal Structure
 */
struct csv_read_internal {
        FILE* file;
        char* buffer;
        size_t bufsize;
        size_t len;
        uint delimLen;

        /* Statistics */
        uint rows;
        uint embedded_breaks;

        /* Properties */
        int normorg;
};

struct csv_internal {
        char* buffer;
        size_t bufidx;
        size_t bufsize;
        int allocated;
};


/**
 * Internal prototypes
 */

/**
 * csv_determine_delimiter chooses between comma, pipe, semi-colon,
 * colon or tab depending on which  one is found most. If none of
 * these delimiters are found, use comma. If this->delimiter was set
 * externally, simply update this->_in->delimLen and return.
 */
void csv_determine_delimiter(struct csv_reader*, const char* header, unsigned char_limit);

/**
 * csv_record_grow allocates space for the fields
 * member (char**) of the csv_record struct
 */
void csv_record_grow(struct csv_record*);

/**
 * csv_append_line is used to retrieve another line
 * of input for an in-line break.
 */
int csv_append_line(struct csv_reader* this, struct csv_record *rec, size_t lineIdx, uint nlCount);

/**
 * Simple CSV parsing. Disregard all quotes.
 */
int csv_parse_none(struct csv_reader*, struct csv_record* rec, const char** line, size_t* lineIdx, unsigned char_limit);

/**
 * Parse line respecting quotes. Quotes within
 * the field are not expected to be duplicated.
 * Leading spaces will cause the field to not
 * be treated as qualified.
 */
int csv_parse_weak(struct csv_reader*, struct csv_record* rec, const char** line, size_t* lineIdx, unsigned char_limit);

/**
 * Parse line according to RFC-4180 guidelines.
 * More info: https://tools.ietf.org/html/rfc4180
 */
int csv_parse_rfc4180(struct csv_reader* this, struct csv_record* rec, const char** line, size_t* lineIdx, unsigned char_limit);


struct csv_reader* csv_reader_new()
{
        init_sig();

        struct csv_reader* reader = NULL;

        malloc_(reader, sizeof(*reader));
        *reader = (struct csv_reader) {
                 NULL           /* internal */
                ,""             /* delimiter */
                ,"\n"           /* embedded_break */
                ,QUOTE_RFC4180  /* quotes */
                ,0              /* Normal */
                ,false          /* failsafeMode */
                ,false
        };

        malloc_(reader->_in, sizeof(*reader->_in));
        *reader->_in = (struct csv_read_internal) {
                 stdin /* file */
                ,NULL  /* buffer */
                ,0     /* bufsize */
                ,0     /* len */
                ,0     /* delimLen */
                ,0     /* rows */
                ,0     /* embedded_breaks */
                ,0     /* normorg */
        };

        //increase_buffer(&reader->_in->buffer, &reader->_in->bufsize);
        //malloc_(reader->_in->_record, sizeof(struct csv_record));

        return reader;
}

void csv_reader_free(struct csv_reader* this)
{
        free_(this->_in->buffer);
        free_(this->_in);
        free_(this);
}

/**
 * Simple accessors
 */
uint csv_reader_row_count(struct csv_reader* this)
{
        return this->_in->rows;
}

uint csv_reader_embedded_breaks(struct csv_reader* this)
{
        return this->_in->embedded_breaks;
}

void csv_record_grow(struct csv_record* this)
{
        ++this->_in->allocated;
        uint arraySize = this->_in->allocated * sizeof(char*);
        if (this->_in->allocated > 1) {
                realloc_(this->fields, arraySize);
        } else {
                malloc_(this->fields, arraySize);
        }
}

int csv_get_record(struct csv_reader* this, struct csv_record* rec)
{
        return csv_nget_record(this, rec, UINT_MAX);
}

int csv_nget_record(struct csv_reader* this, struct csv_record* rec, unsigned char_limit)
{
        return csv_nget_record_to(this, rec, char_limit, UINT_MAX);
}

int csv_get_record_to(struct csv_reader* this, struct csv_record* rec, unsigned field_limit)
{
        return csv_nget_record_to(this, rec, UINT_MAX, field_limit);
}

int csv_nget_record_to(struct csv_reader* this, struct csv_record* rec, unsigned char_limit, unsigned field_limit)
{
        int ret = sgetline(this->_in->file,
                           &this->_in->buffer,
                           &this->_in->bufsize,
                           &this->_in->len);

        if (ret == EOF) {
                this->normal = this->_in->normorg;
                if (csv_reader_close(this) == CSV_FAIL)
                        csv_perror();
                return ret;
        }

        return csv_nparse_to(this, rec, this->_in->buffer, char_limit, field_limit);
}

int csv_lowerstandard(struct csv_reader* this)
{
        if (!this->failsafe_mode || this->_in->file == stdin) {
                switch(this->quotes) {
                case QUOTE_ALL:
                case QUOTE_RFC4180:
                        fprintf(stderr,
                                "Line %d: RFC4180 Qualifier issue.\n",
                                1 + this->_in->rows + this->_in->embedded_breaks);
                        break;
                case QUOTE_WEAK:
                        fprintf(stderr,
                                "Line %d: Qualifier issue.\n",
                                1 + this->_in->rows + this->_in->embedded_breaks);
                        break;
                default:
                        fputs("Unexpected Condition.\n", stderr);
                }

                return CSV_FAIL;
        }

        switch(this->quotes) {
        case QUOTE_ALL:
        case QUOTE_RFC4180:
                fprintf(stderr,
                        "Line %d: Qualifier issue. RFC4180 quotes disabled.\n",
                        1 + this->_in->rows + this->_in->embedded_breaks);
                break;
        case QUOTE_WEAK:
                fprintf(stderr,
                        "Line %d: Qualifier issue. Quotes disabled.\n",
                        1 + this->_in->rows + this->_in->embedded_breaks);
                break;
        default:
                fputs("Unexpected Condition.\n", stderr);
                return CSV_FAIL;
        }

        --this->quotes;
        this->_in->rows = 0;
        this->_in->embedded_breaks = 0;
        fseek(this->_in->file, 0, SEEK_SET);

        return CSV_RESET;
}

void csv_append_empty_field(struct csv_record* this)
{
        if (++(this->size) > this->_in->allocated)
                csv_record_grow(this);

        if (this->_in->bufidx >= this->_in->bufsize)
                increase_buffer(&this->_in->buffer,
                                &this->_in->bufsize);

        this->_in->buffer[this->_in->bufidx] = '\0';
        this->fields[this->size-1] = &this->_in->buffer[this->_in->bufidx];
}


int csv_parse(struct csv_reader *this, struct csv_record *rec, const char* line)
{
        return csv_nparse_to(this, rec, line, UINT_MAX, UINT_MAX);
}

int csv_nparse(struct csv_reader *this, struct csv_record *rec, const char* line, unsigned char_limit)
{
        return csv_nparse_to(this, rec, line, char_limit, UINT_MAX);
}

int csv_parse_to(struct csv_reader *this, struct csv_record *rec, const char* line, unsigned field_limit)
{
        return csv_nparse_to(this, rec, line, UINT_MAX, field_limit);
}

int csv_nparse_to(struct csv_reader *this, struct csv_record *rec, const char* line, unsigned char_limit, unsigned field_limit)
{
        if (!this->_in->delimLen)
                csv_determine_delimiter(this, line, char_limit);

        unsigned line_len = (char_limit == UINT_MAX) ? strlen(line) : char_limit;

        if (line_len >= rec->_in->bufsize) {
                increase_buffer_to(&rec->_in->buffer,
                                   &rec->_in->bufsize,
                                   line_len+1);
        }

        rec->_in->bufidx = 0;
        rec->size = 0;
        rec->raw = NULL;
        rec->raw_len = 0;
        size_t lineIdx = 0;
        int ret = 0;

        while(line[lineIdx] != '\0' && lineIdx < char_limit && rec->size < field_limit) {
                csv_append_empty_field(rec);

                int quotes = this->quotes;
                if (this->quotes && line[lineIdx] != '"')
                        quotes = QUOTE_NONE;
                switch(quotes) {
                case QUOTE_ALL:
                        /* Not seeing a point to implementing this for reading. */
                case QUOTE_RFC4180:
                        ret = csv_parse_rfc4180(this, rec, &line, &lineIdx, char_limit);
                        break;
                case QUOTE_WEAK:
                        ret = csv_parse_weak(this, rec, &line, &lineIdx, char_limit);
                        break;
                case QUOTE_NONE:
                        ret = csv_parse_none(this, rec, &line, &lineIdx, char_limit);
                        break;
                }

                if (ret == CSV_RESET) {
                        ret = csv_lowerstandard(this);
                        csv_reader_reset(this);
                        return ret;
                }
        }

        rec->raw = line; 
        rec->raw_len = line_len;

        if (this->normal > 0) {
                /* Append fields if we are short */
                while (this->normal > rec->size)
                        csv_append_empty_field(rec);
                rec->size = this->normal;
        }

        if (this->normal == CSV_NORMAL_OPEN)
                this->normal = rec->_in->allocated;

        ++this->_in->rows;



        return 0;
}

int csv_append_line(struct csv_reader* this, struct csv_record *rec, size_t lineIdx, uint nlCount)
{
        int ret = sappline(this->_in->file,
                           &this->_in->buffer,
                           &this->_in->bufsize,
                           &this->_in->len);

        uint nlLen = strlen(this->embedded_break);
        uint delta = this->_in->len - lineIdx + (nlLen+1) * nlCount;
        if (delta >= rec->_in->bufsize - rec->_in->bufidx) {
                increase_buffer_to(&rec->_in->buffer,
                                   &rec->_in->bufsize,
                                    rec->_in->bufsize + delta);

                /* Move all pointers from _record->fields */
                int i = rec->size - 1;
                for (; i >= 0; --i) {
                        int distance = rec->fields[i] - rec->fields[0];
                        rec->fields[i] = rec->_in->buffer + distance;
                }
        }

        strcpy(&rec->_in->buffer[rec->_in->bufidx], this->embedded_break);
        rec->_in->bufidx += nlLen;

        return ret;
}

int csv_parse_rfc4180(struct csv_reader* this, struct csv_record* rec, const char** line, size_t* lineIdx, unsigned char_limit)
{
        uint delimIdx = 0;
        uint lastDelimIdx = 0;
        uint qualified = false;
        uint lastWasQuote = false;
        uint trailingSpace = 0;
        uint nlCount = 0;
        int skip = false;
        int appendField = false;
        int firstChar = true;

        while (true) {
                for (; *lineIdx < char_limit && (*line)[*lineIdx] != '\0' && delimIdx != this->_in->delimLen; ++(*lineIdx)) {
                        skip = false;
                        if (qualified) {
                                qualified = ((*line)[*lineIdx] != '"');
                                if (!qualified) {
                                        skip = true;
                                        lastWasQuote = true;
                                }
                        } else {
                                if ((*line)[*lineIdx] == this->delimiter[delimIdx])
                                        ++delimIdx;
                                else
                                        delimIdx = ((*line)[*lineIdx] == this->delimiter[0]) ? 1 : 0;

                                if ( (qualified = ((*line)[*lineIdx] == '"') ) ) {
                                        if (!lastWasQuote)
                                                skip = true;
                                }
                                lastWasQuote = false;
                        }

                        if (!skip) {
                                if (!firstChar || !this->trim || !isspace((*line)[*lineIdx])) {
                                        rec->_in->buffer[rec->_in->bufidx++] = (*line)[*lineIdx];

                                        firstChar = false;
                                        if (this->trim && isspace((*line)[*lineIdx]))
                                                ++trailingSpace;
                                        else if (delimIdx == lastDelimIdx)
                                                trailingSpace = 0;
                                }
                        }

                        if ((*line)[*lineIdx] == '\n')
                                ++this->_in->embedded_breaks;

                        lastDelimIdx = delimIdx;
                }
                /** In-line break found **/
                if ((*line)[*lineIdx] == '\0' && qualified) {
                        if (++nlCount > CSV_MAX_NEWLINES ||
                            csv_append_line(this, rec, *lineIdx, nlCount) == EOF) {
                                return CSV_RESET;
                        }
                        *line = this->_in->buffer;
                        continue;
                }

                break;
        }

        if (delimIdx == this->_in->delimLen) {
                /* Handle Trailing delimiter */
                if (!((*line)[*lineIdx]) || *lineIdx == char_limit)
                        appendField = true;
                rec->_in->bufidx -= this->_in->delimLen;
        }

        rec->_in->bufidx -= trailingSpace;
        rec->_in->buffer[rec->_in->bufidx++] = '\0';

        if (appendField)
                csv_append_empty_field(rec);

        this->_in->embedded_breaks += nlCount;
        return CSV_GOOD;
}

int csv_parse_weak(struct csv_reader* this, struct csv_record* rec, const char** line, size_t* lineIdx, unsigned char_limit)
{
        uint delimIdx = 0;
        uint lastDelimIdx = 0;
        uint onQuote = false;
        uint trailingSpace = 0;
        int nlCount = 0;
        int appendField = false;
        int firstChar = true;

        ++(*lineIdx);

        while (true) {
                for (; *lineIdx < char_limit && (*line)[*lineIdx] != '\0' && delimIdx != this->_in->delimLen; ++(*lineIdx)) {
                        if (onQuote && (*line)[*lineIdx] == this->delimiter[delimIdx]) {
                                ++delimIdx;
                        } else {
                                delimIdx = 0;
                                /* Handle trailing space after quote */
                                //if (onQuote && isspace((*line)[*lineIdx])) {
                                //        ++trailingSpace;
                                //} else {
                                //        trailingSpace = 0;
                                        onQuote = ((*line)[*lineIdx] == '"');
                                //}
                        }
                        if (!firstChar || !this->trim || !isspace((*line)[*lineIdx])) {
                                rec->_in->buffer[rec->_in->bufidx++] = (*line)[*lineIdx];

                                firstChar = false;
                                if (this->trim && isspace((*line)[*lineIdx]))
                                        ++trailingSpace;
                                else if (onQuote + delimIdx == lastDelimIdx)
                                        trailingSpace = 0;
                        }

                        if ((*line)[*lineIdx] == '\n')
                                ++this->_in->embedded_breaks;

                        lastDelimIdx = delimIdx + onQuote;
                }
                if ((*line)[*lineIdx] == '\0' && !onQuote) {
                        if (++nlCount > CSV_MAX_NEWLINES ||
                            csv_append_line(this, rec, *lineIdx, nlCount) == EOF) {
                                return CSV_RESET;
                        }
                        *line = this->_in->buffer;
                        continue;
                }

                break;
        }

        if (delimIdx == this->_in->delimLen) {
                /* Handle Trailing delimiter */
                if (!((*line)[*lineIdx]) || *lineIdx == char_limit)
                        appendField = true;
                rec->_in->bufidx -= this->_in->delimLen;
        }
        rec->_in->bufidx -= trailingSpace + 1;
        rec->_in->buffer[rec->_in->bufidx++] = '\0';

        if (appendField)
                csv_append_empty_field(rec);

        this->_in->embedded_breaks += nlCount;
        return CSV_GOOD;
}

int csv_parse_none(struct csv_reader* this, struct csv_record* rec, const char** line, size_t* lineIdx, unsigned char_limit)
{
        uint delimIdx = 0;
        uint lastDelimIdx = 0;
        uint trailingSpace = 0;
        int appendField = false;
        int firstChar = true;

        for (; *lineIdx < char_limit && (*line)[*lineIdx] != '\0' && delimIdx != this->_in->delimLen; ++(*lineIdx)) {
                if ((*line)[*lineIdx] == this->delimiter[delimIdx])
                        ++delimIdx;
                else if (delimIdx != 0)
                        delimIdx = ((*line)[*lineIdx] == this->delimiter[0]) ? 1 : 0;

                if (!firstChar || !this->trim || !isspace((*line)[*lineIdx])) {
                        rec->_in->buffer[rec->_in->bufidx++] = (*line)[*lineIdx];

                        firstChar = false;
                        if (this->trim && isspace((*line)[*lineIdx]))
                                ++trailingSpace;
                        else if (delimIdx == lastDelimIdx)
                                trailingSpace = 0;
                }


                if ((*line)[*lineIdx] == '\n')
                        ++this->_in->embedded_breaks;

                lastDelimIdx = delimIdx;
        }

        if (delimIdx == this->_in->delimLen) {
                /* Handle Trailing delimiter */
                if (!((*line)[*lineIdx]) || *lineIdx == char_limit)
                        appendField = true;
                rec->_in->bufidx -= this->_in->delimLen;
        }

        rec->_in->bufidx -= trailingSpace;
        rec->_in->buffer[rec->_in->bufidx++] = '\0';

        if (appendField)
                csv_append_empty_field(rec);

        return CSV_GOOD;
}

void csv_determine_delimiter(struct csv_reader* this, const char* header, unsigned char_limit)
{
        uint delimLen = strlen(this->delimiter);
        if (delimLen) {
                this->_in->delimLen = delimLen;
                return;
        }

        const char* delims = ",|\t;:";
        uint i = 0;
        int sel = 0;
        int count = 0;
        int maxCount = 0;
        for (; i < strlen(delims); ++i) {
                count = charncount(header, delims[i], char_limit);
                if (count > maxCount) {
                        sel = i;
                        maxCount = count;
                }
        }

        this->delimiter[0] = delims[sel];
        this->delimiter[1] = '\0';
        this->_in->delimLen = 1;
}

int csv_reader_open(struct csv_reader* this, const char* fileName)
{
        this->_in->file = fopen(fileName, "r");
        FAIL_IF(!this->_in->file, fileName);
        return CSV_GOOD;
}

int csv_reader_close(struct csv_reader* this)
{
        if (this->_in->file && this->_in->file != stdin) {
                int ret = fclose(this->_in->file);
                FAIL_IF(ret, "fclose");
        }
        return CSV_GOOD;
}

int csv_reader_reset(struct csv_reader* this)
{
        this->normal = this->_in->normorg;
        this->delimiter[0] = '\0';
        this->_in->delimLen = 0;
        this->_in->embedded_breaks = 0;
        this->_in->rows = 0;
        //this->_in->buffer[0] = '\0';
        if (this->_in->file && this->_in->file != stdin) {
                int ret = fseek(this->_in->file, 0, SEEK_SET);
                FAIL_IF(ret, "fseek");
        }
        return CSV_GOOD;
}
