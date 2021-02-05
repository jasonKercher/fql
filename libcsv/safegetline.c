#include "safegetline.h"
#include "misc.h"

#include <string.h>

int _safegetline(FILE *fp, char* buffer, size_t* buflen, size_t* off)
{
        char *end = buffer + *buflen - 1;
        char *dst = buffer + *off;
        int endfound = false;
        int c = 0;

        while (!endfound && dst < end && (c = getc(fp)) != EOF)
        {
                *dst++ = c;
                if (c == '\r') {
                        c = getc(fp);
                        if (c != '\n' && c != EOF)
                                ungetc(c, fp);
                        endfound = true;
                }
                if (c == '\n') {
                        /* Handles trailing EOL at EOF consistently */
                        c = getc(fp);
                        if (c != EOF)
                                ungetc(c, fp);
                        endfound = true;
                }
        }

        if (endfound)
                dst--;

        *dst = '\0';
        *off = dst - buffer;

        /* End of Buffer before End of Line */
        if (dst == end)
                return EOF - 1;

        if (c == EOF && dst == buffer) {
                if (ferror(fp)) {
                        perror("getc");
                        exit(EXIT_FAILURE);
                }
                return EOF;
        }

        return 0;
}

int _getline_runner(FILE* f, char** buf, size_t* buflen, size_t* len, size_t off)
{
        size_t offset = off;
        int ret = 0;
        do {
                if(offset + 1 >= *buflen)
                        increase_buffer(buf, buflen);
                ret = _safegetline(f, *buf, buflen, &offset);
        } while (ret == EOF - 1);

        if (len)
                *len = offset;

        return ret;
}

int sappline(FILE *f, char **buf, size_t* buflen, size_t* len)
{
        return _getline_runner(f, buf, buflen, len, strlen(*buf));
}

int sgetline(FILE *f, char **buf, size_t* buflen, size_t* len)
{
        return _getline_runner(f, buf, buflen, len, 0);
}
