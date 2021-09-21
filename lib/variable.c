#include "variable.h"
#include "util/util.h"
#include <string.h>

variable* variable_construct(variable* self)
{
	memset(self, 0, sizeof(*self));
	return self;
}

void variable_destroy(variable* self)
{
	delete_if_exists_(string, self->_data);
}

void _append_space(variable* self)
{
	if (self->limit == 0) {
		return;
	}
	while (self->_data->size < self->limit) {
		string_push_back(self->_data, ' ');
	}
}

void variable_clear(variable* self)
{
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

void variable_set_string(variable* self, const string* s)
{
	if (self->_data == NULL) {
		self->_data = string_from_string(s);
		return;
	}

	string_copy(self->_data, s);
	self->value.s = self->_data;
	if (self->type == SQL_CHAR) {
		_append_space(self);
	}
}

void variable_set_stringview(variable* self, const stringview* sv)
{
	if (self->_data == NULL) {
		self->_data = string_from_stringview(sv);
		return;
	}

	string_copy_from_stringview(self->_data, sv);
	self->value.s = self->_data;
	if (self->type == SQL_CHAR) {
		_append_space(self);
	}
}

void variable_copy(variable* self, const variable* src)
{
	//self->type = src->type;
	//switch (self->type) {
	//case FIELD_INT:
	//	self->value.i = src->value.i;
	//	break;
	//case FIELD_FLOAT:
	//	self->value.f = src->value.f;
	//	break;
	//case FIELD_STRING:
	//	variable_set_string(self, src->value.s);
	//	break;
	//case FIELD_UNDEFINED:;
	//}
}
