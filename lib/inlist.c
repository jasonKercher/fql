#include "logic.h"
#include "column.h"
#include "util/util.h"

inlist* inlist_construct(inlist* self)
{
	*self = (inlist) {
		NULL, /* columns */
		NULL, /* list_data */
	};
	return self;
}

void inlist_destroy(inlist* self)
{
	if (self->columns != NULL) {
		delete_(vec, self->columns);
	}

	if (self->list_data != NULL) {
		delete_(set, self->list_data);
	}
}

void inlist_add_column(inlist* self, column* col)
{
	if (self->columns == NULL) {
		self->columns = new_t_(vec, column*);
	}
	vec_push_back(self->columns, &col);
}

enum field_type inlist_determine_type(inlist* self, column* left_side)
{
	enum field_type type = left_side->field_type;
	column** it = vec_begin(self->columns);
	for (; it != vec_end(self->columns); ++it) {
		type = field_determine_type(type, (*it)->field_type);
	}

	return type;
}

void inlist_cat_description(inlist* self, string* msg)
{
	if (self->list_data != NULL) {
		string_strcat(msg, " <subquery>");
		return;
	}
	column** begin = vec_begin(self->columns);
	column** it = begin;
	for (; it != vec_end(self->columns); ++it) {
		if (it != begin) {
			string_strcat(msg, ", ");
		}
		column_cat_description(*it, msg);
	}
}
