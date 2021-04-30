#include "group.h"
#include "column.h"
#include "query.h"
#include "util/util.h"
#include "util/stringview.h"

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

int aggregate_resolve(Aggregate* agg, enum aggregate_function agg_type)
{
	switch (agg_type) {
	default:
		fprintf(stderr, "Aggregate function `%s' not yet implemented\n", agg_str[agg_type]);
		return FQL_FAIL;
	}
}

Group* group_new()
{
	Group* new_group = NULL;
	malloc_(new_group, sizeof(*new_group));

	return group_construct(new_group);
}

Group* group_construct(Group* group)
{
	vec_construct_(&group->columns, Column*);
	vec_construct_(&group->aggregates, Aggregate);
	vec_construct_(&group->_indicies, size_t);
	vec_construct_(&group->_raw, char);
	vec_construct_(&group->_composite, StringView);
	compositemap_construct_(&group->groups,
			        unsigned,
				128,
				HASHMAP_PROP_NOCASE | HASHMAP_PROP_RTRIM);

	return group;
}

void group_free(Group* group)
{
	if (group == NULL) {
		return;
	}
	group_destroy(group);
	free_(group);
}
void group_destroy(Group* group)
{
	vec_destroy(&group->columns);
	vec_destroy(&group->aggregates);
	vec_destroy(&group->_indicies);
	vec_destroy(&group->_raw);
	vec_destroy(&group->_composite);
	compositemap_destroy(&group->groups);
}

void group_add_column(Group* group, Column* col)
{
	vec_push_back(&group->columns, &col);
	vec_add_one(&group->_composite);
}

void group_cat_description(Group* group, Process* proc)
{
	Column** it = vec_begin(&group->columns);
	for (; it != vec_end(&group->columns); ++it) {
		if (it != vec_begin(&group->columns)) {
			string_strcat(proc->action_msg, ",");
		}
		column_cat_description(*it, proc->action_msg);
	}
}

int group_record(Group* group, Vec* recs)
{
	size_t raw_size = group->_raw.size;
	size_t running_size = raw_size;

	Column** cols = vec_begin(&group->columns);
	int i = 0;
	long num_i = 0;
	double num_f = 0;
	for (; i < group->columns.size; ++i) {
		StringView* sv = vec_at(&group->_composite, i);
		switch (cols[i]->field_type) {
		case FIELD_STRING:
			if (column_get_stringview(sv, cols[i], recs)) {
				return FQL_FAIL;
			}
			vec_append(&group->_raw, sv->data, sv->len);
			break;
		case FIELD_INT:
			if (column_get_int(&num_i, cols[i], recs)) {
				return FQL_FAIL;
			}
			vec_append(&group->_raw, &num_i, sizeof(long));
			sv->data = (char*)vec_end(&group->_raw) - sizeof(long);
			sv->len = sizeof(long);
			break;
		case FIELD_FLOAT:
			if (column_get_float(&num_f, cols[i], recs)) {
				return FQL_FAIL;
			}
			vec_append(&group->_raw, &num_f, sizeof(double));
			sv->data = (char*)vec_end(&group->_raw) - sizeof(double);
			sv->len = sizeof(double);
		default:
			;
		}

		vec_push_back(&group->_indicies, &running_size);
		running_size += sv->len;
	}

	unsigned group_count = group->groups.values.size;
	unsigned* idx_ptr = compositemap_get(&group->groups, &group->_composite);
	unsigned idx = 0;
	int ret = 0;
	if (idx_ptr == NULL) {
		idx = group_count;
		compositemap_set(&group->groups, &group->_composite, &group_count);
		ret = 1;
	} else {
		idx = *idx_ptr;
		vec_resize(&group->_raw, raw_size);
		vec_resize(&group->_indicies,
		group_count / group->columns.size);
	}

	return ret;
}