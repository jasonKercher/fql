#include "stringy.h"

#include <string.h>
#include "stringview.h"
#include "util.h"

String* string_new()
{
        /* don't call string_construct since
         * vec_new will call vec_construct
         */
        String* new_string = vec_new_(char);
        char* s = new_string->data;
        s[0] = '\0';

        return new_string;
}

String* string_construct(String* string)
{
        string = vec_construct(string, 1);
        char* s = string->data;
        s[0] = '\0';
        return string;
}

String* string_from_string(String* src)
{
        String* new_string = string_new();
        string_copy(new_string, src);
        return new_string;
}

String* string_construct_from_string(String* dest, String* src)
{
        string_construct(dest);
        string_copy(dest, src);
        return dest;
}

String* string_from_stringview(struct stringview* sv)
{
        String* new_string = string_new();
        string_copy_from_stringview(new_string, sv);
        return new_string;
}

String* string_construct_from_stringview(String* s, struct stringview* sv)
{
        string_construct(s);
        string_copy_from_stringview(s, sv);
        return s;
}

String* string_from_char_ptr(const char* src)
{
        String* new_string = vec_new_(char);
        string_strcpy(new_string, src);
        return new_string;
}

String* string_construct_from_char_ptr(String* s, const char* cp)
{
        string_construct(s);
        string_strcpy(s, cp);
        return s;
}

String* string_take(char* src)
{
        String* new_string = NULL;
        malloc_(new_string, sizeof(*new_string));

        return string_construct_take(new_string, src);
}

String* string_construct_take(String* s, char* src)
{
        int len = strlen(src);

        /* I'm making an assumption about _alloc here.
         * I don't know how much space is allocated,
         * but it is >= len + 1
         */
        *s = (String) {
                 src            /* data */
                ,len            /* size */
                ,len + 1        /* _alloc */
                ,1              /* _elem_s */
        };

        return s;
}

void string_copy_from_stringview(String* s, struct stringview* sv)
{
        string_strncpy(s, sv->data, sv->len);
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

void string_strcat(String* dest, const char* src)
{
        int len = strlen(src);
        int index = dest->size;
        vec_resize(dest, dest->size + len);
        void* end = vec_at(dest, index);
        memcpy(end, src, len + 1);
}

void string_strcpy(String* dest, const char* src)
{
        size_t len = strlen(src);
        vec_resize(dest, len);
        memcpy(dest->data, src, len + 1);
}

void string_strncpy(String* dest, const char* src, size_t len)
{
        vec_resize(dest, len);
        size_t i = 0;
        for (; src[i] != '\0' && i < len; ++i) {
                ((char*)dest->data)[i] = src[i];
        }
        vec_resize(dest, i);
        ((char*) dest->data)[i] = '\0';
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

void string_copy(String* dest, String* src)
{
        vec_resize(dest, src->size);
        memcpy(dest->data, src->data, src->size);
        ((char*) dest->data)[dest->size] = '\0';
}
