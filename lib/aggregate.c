#include "aggregate.h"
#include <stdio.h>
#include "util/util.h"

static const char* agg_str[14] = {
        "UNDEFINED",
        "AVG",
        "CHECKSUM_AGG",
        "COUNT",
        "COUNT_BIG",
        "GROUPING",
        "GROUPING_ID",
        "MAX",
        "MIN",
        "STDEV",
        "STDEVP",
        "SUM",
        "VAR",
        "VARP",
};

aggregate* aggregate_construct(aggregate* self,
                               enum aggregate_function agg_type)
{
	*self = (aggregate) {
	        .args = new_t_(vec, expression*),
	        .agg_type = agg_type,
	        .data_type = FIELD_UNDEFINED,
	        .return_mode = MODE_UNDEFINED,
	};

	vec_construct(&self->results, sizeof(struct aggresult));

	return self;
}

void aggregate_destroy(aggregate* self)
{
	expression** it = vec_begin(self->args);
	for (; it != vec_end(self->args); ++it) {
		delete_(expression, *it);
	}
	delete_(vec, self->args);
	if (self->data_type == FIELD_STRING) {
		struct aggresult* it = vec_begin(&self->results);
		for (; it != vec_end(&self->results); ++it) {
			string_destroy(&it->data.s);
		}
	}
	vec_destroy(&self->results);
}

void aggregate_reset(aggregate* self)
{
	if (self->data_type == FIELD_STRING) {
		struct aggresult* it = vec_begin(&self->results);
		for (; it != vec_end(&self->results); ++it) {
			string_destroy(&it->data.s);
		}
	}
	vec_clear(&self->results);
}

const char* aggregate_get_name(aggregate* self)
{
	return agg_str[self->agg_type];
}

void aggregate_add_expression(aggregate* self, expression* col)
{
	vec_push_back(self->args, &col);
}

int aggregate_resolve(aggregate* self, expression* col)
{
	if (self->call__ != NULL) {
		return FQL_GOOD;
	}
	expression* argument = *(expression**)vec_begin(self->args);
	switch (self->agg_type) {
	case AGG_COUNT:
		self->call__ = &fql_count;
		self->data_type = FIELD_INT;
		break;
	case AGG_MIN:
		switch (argument->field_type) {
		case FIELD_INT:
			self->call__ = &fql_min_i;
			break;
		case FIELD_FLOAT:
			self->call__ = &fql_min_f;
			break;
		case FIELD_STRING:
			self->call__ = &fql_min_s;
			break;
		default:
			goto unexpected_type;
		}
		self->data_type = argument->field_type;
		break;
	case AGG_MAX:
		switch (argument->field_type) {
		case FIELD_INT:
			self->call__ = &fql_max_i;
			break;
		case FIELD_FLOAT:
			self->call__ = &fql_max_f;
			break;
		case FIELD_STRING:
			self->call__ = &fql_max_s;
			break;
		default:
			goto unexpected_type;
		}
		self->data_type = argument->field_type;
		break;
	case AGG_SUM:
		switch (argument->field_type) {
		case FIELD_INT:
			self->call__ = &fql_sum_i;
			break;
		case FIELD_FLOAT:
			self->call__ = &fql_sum_f;
			break;
		default:
			goto unexpected_type;
		}
		self->data_type = argument->field_type;
		break;
	case AGG_AVG:
		switch (argument->field_type) {
		case FIELD_INT:
			self->call__ = &fql_avg_i;
			break;
		case FIELD_FLOAT:
			self->call__ = &fql_avg_f;
			break;
		default:
			goto unexpected_type;
		}
		self->data_type = argument->field_type;
		break;
	default:
		fprintf(stderr,
		        "Aggregate function `%s' not yet implemented\n",
		        agg_str[self->agg_type]);
		return FQL_FAIL;
	}

	col->field_type = self->data_type;
	return FQL_GOOD;

unexpected_type:
	fprintf(stderr,
	        "unexpected type `%s' in aggregate `%s'\n",
	        field_description(argument->field_type),
	        agg_str[self->agg_type]);
	return FQL_FAIL;
}
