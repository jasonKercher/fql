#include "fqlstring.h"

#include <string.h>
#include "stringview.h"
#include "util.h"

String* string_new()
{
        String* new_string = vec_new_(char);
        char* s = new_string->data;
        s[0] = '\0';

        return new_string;
}

String* string_init(String* string)
{
        string = vec_init(string, 1);
        char* s = string->data;
        s[0] = '\0';
        return string;
}

String* string_from_char_ptr(const char* src)
{
        String* new_string = vec_new_(char);
        string_cpy(new_string, src);
        return new_string;
}

String* string_take(char* src)
{
        String* new_string = NULL;
        malloc_(new_string, sizeof(*new_string));

        int len = strlen(src);

        /* I'm making an assumption about _alloc here.
         * I don't know how much space is allocated,
         * but it is >= len + 1
         */
        *new_string = (String) {
                 src            /* data */
                ,len            /* size */
                ,len + 1        /* _alloc */
                ,1              /* _elem_s */
        };

        return new_string;
}

void string_copy_from_stringview(String* s, struct stringview* sv)
{
        vec_resize(s, sv->len);
        memcpy(s->data, sv->data, sv->len);
        ((char*) s->data)[s->size] = '\0';
}

void string_append_stringview(String* dest, struct stringview* sv)
{
        int index = dest->size;
        vec_resize(dest, dest->size + sv->len);
        void* end = vec_at(dest, index);
        memcpy(end, sv->data, sv->len);
        ((char*) dest->data)[dest->size] = '\0';
}

void string_push_back(String* s, char c)
{
        char* back = vec_add_one(s);
        *back = c;
        back[1] = '\0';
}

void string_cat(String* dest, const char* src)
{
        int len = strlen(src);
        int index = dest->size;
        vec_resize(dest, dest->size + len);
        void* end = vec_at(dest, index);
        memcpy(end, src, len + 1);
}

void string_cpy(String* dest, const char* src)
{
        int len = strlen(src);
        vec_resize(dest, len);
        memcpy(dest->data, src, len + 1);
}

void string_sprintf(String* s, const char* fmt, ...)
{
        va_list args;
        va_start(args, fmt);
        va_list args2;
        va_copy(args2, args);
        int len = vsnprintf(NULL, 0, fmt, args);
        va_end(args);
        vec_resize(s, len+1);
        vsnprintf(s->data, len+1, fmt, args2);
        va_end(args2);
}
