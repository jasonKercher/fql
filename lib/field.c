#include "field.h"
#include "fql.h"
#include "util/util.h"

static const char* _int_descript = "INT";
static const char* _float_descript = "FLOAT";
static const char* _string_descript = "STRING";
static const char* _unknown_descript = "<unknown>";

const char* field_description(enum field_type type)
{
	switch (type) {
	case FIELD_INT:
		return _int_descript;
	case FIELD_FLOAT:
		return _float_descript;
	case FIELD_STRING:
		return _string_descript;
	default:
		return _unknown_descript;
	}
}

enum field_type field_determine_type(enum field_type t0, enum field_type t1)
{
	if (t0 == t1) {
		return t0;
	} else if (t0 == FIELD_STRING) {
		return t1;
	} else if (t1 == FIELD_STRING) {
		return t0;
	}
	return FIELD_FLOAT;
}

int field_to_int(long* ret, union field* field, enum field_type* type)
{
	switch (*type) {
	case FIELD_INT:
		break;
	case FIELD_FLOAT:
		*type = FIELD_INT;
		field->i = (long)field->f;
		break;
	case FIELD_STRING:
		*type = FIELD_INT;
		long val = 0;
		if (str2long(&val, field->s->data)) {
			return FQL_FAIL;
		}
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
		field->f = (double)field->i;
		break;
	case FIELD_STRING:
		*type = FIELD_FLOAT;
		double val = 0;
		if (str2double(&val, field->s->data)) {
			return FQL_FAIL;
		}
		field->f = val;
		break;
	default:
		return FQL_FAIL;
	}

	*ret = field->f;
	return FQL_GOOD;
}

int field_to_stringview(stringview* ret,
                        string* dest,
                        union field* field,
                        enum field_type* type)
{
	switch (*type) {
	case FIELD_STRING:
		break;
	case FIELD_INT: {
		*type = FIELD_STRING;
		string_sprintf(dest, "%ld", field->i);
		break;
	}
	case FIELD_FLOAT: {
		*type = FIELD_STRING;
		string_sprintf(dest, "%lf", field->f);
		break;
	}
	default:
		return FQL_FAIL;
	}

	stringview_set_string(ret, dest);
	field->s = dest;

	return FQL_GOOD;
}

int field_to_stringview_limited(stringview* ret,
                                string* dest,
                                union field* field,
                                enum field_type* type,
                                unsigned limit)
{
	switch (*type) {
	case FIELD_STRING:
		if (dest->size > limit) {
			string_resize(dest, limit);
		}
		break;
	case FIELD_INT: {
		*type = FIELD_STRING;
		string_sprintf(dest, "%ld", field->i);
		if (dest->size > limit) {
			string_resize(dest, limit);
		}
		break;
	}
	case FIELD_FLOAT: {
		*type = FIELD_STRING;
		string_sprintf(dest, "%lf", field->f);
		if (dest->size > limit) {
			string_resize(dest, limit);
		}
		break;
	}
	default:
		return FQL_FAIL;
	}

	stringview_set_string(ret, dest);
	field->s = dest;

	return FQL_GOOD;
}
