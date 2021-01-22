#include "field.h"
#include "fql.h"
#include "util/util.h"

int field_to_int(long* ret, union field* field, enum field_type* type)
{
        switch (*type) {
        case FIELD_INT:
                break;
        case FIELD_FLOAT:
                *type = FIELD_INT;
                field->i = (long) field->f;
                break;
        case FIELD_STRING:
                *type = FIELD_INT;
                long val = 0;
                if (str2long(&val, field->s->data)) {
                        string_free(field->s);
                        return FQL_FAIL;
                }
                string_free(field->s);
                field->i = val;
                break;
        default:
                return FQL_FAIL;
        }
        *ret = field->i;
        return FQL_GOOD;
}

int field_to_float(double* ret, union field* field, enum field_type* type)
{
        switch (*type) {
        case FIELD_FLOAT:
                break;
        case FIELD_INT:
                *type = FIELD_FLOAT;
                field->f = (double) field->i;
                break;
        case FIELD_STRING:
                *type = FIELD_FLOAT;
                double val = 0;
                if (str2double(&val, field->s->data)) {
                        string_free(field->s);
                        return FQL_FAIL;
                }
                string_free(field->s);
                field->f = val;
                break;
        default:
                return FQL_FAIL;
        }

        *ret = field->f;
        return FQL_GOOD;
}

int field_to_stringview(StringView* ret, union field* field, enum field_type* type)
{
        switch (*type) {
        case FIELD_STRING:
                break;
        case FIELD_INT:
        {
                *type = FIELD_STRING;
                String* s = string_new();
                string_sprintf(s, "%ld", field->i);
                field->s = s;
                break;
        }
        case FIELD_FLOAT:
        {
                *type = FIELD_STRING;
                String* s = string_new();
                string_sprintf(s, "%lf", field->f);
                field->s = s;
                break;
        }
        default:
                return FQL_FAIL;
        }

        stringview_construct_from_string(ret, field->s);
        return FQL_GOOD;
}
