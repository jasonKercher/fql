#include "variable.h"
#include "misc.h"
#include "util/util.h"
#include <string.h>

variable* variable_construct(variable* self)
{
	*self = (variable) {
	        .type = SQL_UNDEFINED,
	        .limit = INT_MAX,
	        .is_null = true,
	};
	return self;
}

void variable_destroy(variable* self)
{
	delete_if_exists_(string, self->_data);
}

void _append_space(variable* self)
{
	if (self->limit == 0 || self->limit == INT_MAX) {
		return;
	}
	while (self->_data->size < self->limit) {
		string_push_back(self->_data, ' ');
	}
}

void variable_clear(variable* self)
{
	self->is_null = true;

	switch (self->type) {
	case SQL_INT:
	case SQL_BIT:
		self->value.i = 0;
		return;
	case SQL_FLOAT:
		self->value.f = 0;
		return;
	case SQL_TEXT:
	case SQL_CHAR:
	case SQL_VARCHAR:
	case SQL_UNDEFINED:;
	}

	string_clear(self->_data);
	if (self->type == SQL_CHAR) {
		_append_space(self);
	}
}

void variable_reset(variable* self)
{
	if (self->_import_value == NULL) {
		variable_clear(self);
		return;
	}

	string_copy(self->_data, self->_import_value);
	self->value.s = self->_data;

	enum sql_type correct_type = self->type;
	self->type = SQL_VARCHAR;
	variable_set_type(self, correct_type);
}

enum field_type variable_get_field_type(enum sql_type sqltype)
{
	switch (sqltype) {
	case SQL_BIT:
	case SQL_INT:
		return FIELD_INT;
	case SQL_FLOAT:
		return FIELD_FLOAT;
	case SQL_TEXT:
	case SQL_CHAR:
	case SQL_VARCHAR:
		return FIELD_STRING;
	case SQL_UNDEFINED:;
	}
	return FIELD_UNDEFINED;
}

int variable_set_type(variable* self, enum sql_type new_type)
{
	if (self->type == new_type) {
		return FQL_GOOD;
	}

	if (self->type == SQL_UNDEFINED) {
		switch (new_type) {
		case SQL_BIT:
		case SQL_INT:
			self->value.i = 0;
			break;
		case SQL_FLOAT:
			self->value.f = 0;
			break;
		case SQL_TEXT:
		case SQL_CHAR:
		case SQL_VARCHAR:
			if (self->_data == NULL) {
				self->_data = new_(string);
			}
			string_clear(self->_data);
			self->value.s = self->_data;
			break;
		case SQL_UNDEFINED:
			fputs("undefined sql type\n", stderr);
			return FQL_FAIL;
		}

		self->type = new_type;
		return FQL_GOOD;
	}

	enum field_type fieldtype = variable_get_field_type(self->type);

	switch (new_type) {
	case SQL_BIT: {
		long n = 0;
		try_(field_to_int(&n, &self->value, &fieldtype));
		break;
	}
	case SQL_INT: {
		long n = 0;
		try_(field_to_int(&n, &self->value, &fieldtype));
		break;
	}
	case SQL_FLOAT: {
		double n = 0;
		try_(field_to_float(&n, &self->value, &fieldtype));
		break;
	}
	case SQL_TEXT:
	case SQL_CHAR:
	case SQL_VARCHAR: {
		if (self->_data == NULL) {
			self->_data = new_(string);
		}
		stringview sv;
		if (self->limit != INT_MAX) {
			try_(field_to_stringview(&sv,
			                         self->_data,
			                         &self->value,
			                         &fieldtype));
		} else {
			try_(field_to_stringview_limited(&sv,
			                                 self->_data,
			                                 &self->value,
			                                 &fieldtype,
			                                 self->limit));
		}
		break;
	}
	case SQL_UNDEFINED:
		fputs("undefined sql type\n", stderr);
		return FQL_FAIL;
	}

	self->type = new_type;
	if (self->type == SQL_CHAR) {
		_append_space(self);
	}

	return FQL_GOOD;
}

void variable_set_string(variable* self, const string* s)
{
	stringview sv = {s->data, s->size};
	variable_set_stringview(self, &sv);
}

void variable_set_stringview(variable* self, stringview* sv)
{
	if (sv->len > self->limit) {
		sv->len = self->limit;
	}

	if (self->_data == NULL) {
		self->_data = string_from_stringview(sv);
		self->value.s = self->_data;
		return;
	}

	string_copy_from_stringview(self->_data, sv);
	self->value.s = self->_data;
	if (self->type == SQL_CHAR) {
		_append_space(self);
	}
}

void variable_set_import_value(variable* self, const char* value)
{
	self->_import_value = string_from_char_ptr(value);
	variable_set_string(self, self->_import_value);
	self->type = SQL_VARCHAR;
}
