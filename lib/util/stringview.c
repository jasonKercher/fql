#include "stringview.h"

#include "util.h"

StringView* stringview_new(char* s, unsigned len)
{
        StringView* new_stringview = NULL;
        malloc_(new_stringview, sizeof(*new_stringview));

        return stringview_init(new_stringview, s, len);
}

StringView* stringview_init(StringView* sv, char* s, unsigned len)
{
        *sv = (StringView) {
                 s
                ,len
        };

        return sv;
}

void stringview_free(StringView* sv)
{
        free_(sv);
}