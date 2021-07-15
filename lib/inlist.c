#include "logic.h"
#include "expression.h"
#include "fqlselect.h"
#include "util/util.h"

inlist* inlist_construct(inlist* self)
{
	*self = (inlist) {
	        NULL,           /* expressions */
	        NULL,           /* subquery */
	        NULL,           /* list_data */
	        MODE_UNDEFINED, /* return_mode */
	};
	return self;
}

void inlist_destroy(inlist* self)
{
	delete_if_exists_(set, self->list_data);
	if (self->expressions == NULL) {
		return;
	}
	expression** it = vec_begin(self->expressions);
	for (; it != vec_end(self->expressions); ++it) {
		delete_(expression, *it);
	}
	delete_(vec, self->expressions);
}

void inlist_add_expression(inlist* self, expression* expr)
{
	if (self->expressions == NULL) {
		self->expressions = new_t_(vec, expression*);
	}
	vec_push_back(self->expressions, &expr);
}

enum field_type inlist_determine_type(inlist* self, expression* left_side)
{
	enum field_type type = left_side->field_type;
	vec* expressions = self->expressions;
	if (self->subquery != NULL) {
		fqlselect* select = self->subquery->op;
		expressions = select->schema->expressions;
	}

	expression** it = vec_begin(expressions);
	for (; it != vec_end(expressions); ++it) {
		type = field_determine_type(type, (*it)->field_type);
	}

	return type;
}

void inlist_cat_description(inlist* self, string* msg)
{
	if (self->subquery != NULL) {
		string_strcat(msg, " <subquery>");
		return;
	}
	expression** begin = vec_begin(self->expressions);
	expression** it = begin;
	for (; it != vec_end(self->expressions); ++it) {
		if (it != begin) {
			string_strcat(msg, ", ");
		}
		expression_cat_description(*it, msg);
	}
}
