#include "group.h"
#include "aggregate.h"
#include "util/util.h"
#include "util/stringview.h"

Group* group_new()
{
	Group* new_group = NULL;
	malloc_(new_group, sizeof(*new_group));

	return group_construct(new_group);
}

Group* group_construct(Group* group)
{
	memset(group, 0, sizeof(*group));
	compositemap_construct_(&group->val_map,
			        unsigned,
				128,
				HASHMAP_PROP_NOCASE | HASHMAP_PROP_RTRIM);
	vec_construct_(&group->columns, Column*);
	vec_construct_(&group->aggregates, Aggregate*);
	vec_construct_(&group->_indicies, size_t);
	vec_construct_(&group->_raw, char);
	vec_construct_(&group->_composite, StringView);

	size_t zero = 0;
	vec_push_back(&group->_indicies, &zero);

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
	if (group->expr_map != NULL) {
		compositemap_free(group->expr_map);
	}
	compositemap_destroy(&group->val_map);
	vec_destroy(&group->columns);
	vec_destroy(&group->aggregates);
	vec_destroy(&group->_indicies);
	vec_destroy(&group->_raw);
	vec_destroy(&group->_composite);
}

void group_add_column(Group* group, Column* col)
{
	col->src_idx = group->columns.size;
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

		/* NOTE: running size is incremented first, because it should
		 *       mark the beginning of the next element. _indicies.size
		 *       should always be number of elements + 1.
		 */
		running_size += sv->len;
		vec_push_back(&group->_indicies, &running_size);
	}

	unsigned group_count = group->val_map.values.size;
	unsigned* idx_ptr = compositemap_get(&group->val_map, &group->_composite);
	unsigned idx = 0;
	int ret = 0;
	if (idx_ptr == NULL) {
		idx = group_count;
		compositemap_set(&group->val_map, &group->_composite, &group_count);
		ret = 1;
	} else {
		idx = *idx_ptr;
		vec_resize(&group->_raw, raw_size);
		vec_resize(&group->_indicies,
		group_count / group->columns.size);
	}

	return ret;
}

int group_dump_record(Group* group, Record* rec)
{
	if (group->_dump_idx + 1 >= group->_indicies.size) {
		return -1;
	}

	size_t* idx = vec_at(&group->_indicies, group->_dump_idx);
	unsigned len = *(idx+1) - *idx;
	
	int i = 0;
	Column** group_cols = vec_begin(&group->columns);
	StringView* rec_svs = vec_begin(rec->fields);
	for (; i < group->columns.size; ++i) {
		if (group_cols[i]->expr == EXPR_AGGREGATE) {
			continue; /* TODO */
		}
		stringview_nset(&rec_svs[i],
				vec_at(&group->_raw, *idx),
				len);
	}

	++group->_dump_idx;
	return FQL_GOOD;
}
