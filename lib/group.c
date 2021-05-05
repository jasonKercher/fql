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
	col->location = group->columns.size;
	vec_push_back(&group->columns, &col);
	if (col->expr != EXPR_AGGREGATE) {
		vec_add_one(&group->_composite);
	}
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

int _add_agg_result(Group* group, Vec* recs)
{
	Aggregate** it = vec_begin(&group->aggregates);
	for (; it != vec_end(&group->aggregates); ++it) {
		struct aggresult* result = vec_add_one(&(*it)->results);
		*result = (struct aggresult) { 0 };
		if ((*it)->data_type == FIELD_STRING) {
			string_construct(&result->data.s);
		}
		(*it)->call__(*it, group, result, recs);
	}
	return FQL_GOOD;
}

int _update_agg_result(Group* group, Vec* recs, unsigned idx)
{
	Aggregate** it = vec_begin(&group->aggregates);
	for (; it != vec_end(&group->aggregates); ++it) {
		struct aggresult* result = vec_at(&(*it)->results, idx);
		(*it)->call__(*it, group, result, recs);
	}
	return FQL_GOOD;
}
int group_record(Group* group, Vec* recs)
{
	size_t raw_size = group->_raw.size;
	size_t index_count = group->_indicies.size;
	size_t running_size = raw_size;

	Column** cols = vec_begin(&group->columns);
	StringView* sv = vec_begin(&group->_composite);
	int i = 0;
	for (; i < group->columns.size; ++i) {
		if (cols[i]->expr == EXPR_AGGREGATE) {
			continue;
		}
		switch (cols[i]->field_type) {
		case FIELD_STRING:
			if (column_get_stringview(sv, cols[i], recs)) {
				return FQL_FAIL;
			}
			vec_append(&group->_raw, sv->data, sv->len);
			break;
		case FIELD_INT:
		 {
			long num_i = 0;
			if (column_get_int(&num_i, cols[i], recs)) {
				return FQL_FAIL;
			}
			size_t old_size = group->_raw.size;
			unsigned len = snprintf(NULL, 0, "%ld", num_i);
			vec_resize(&group->_raw, old_size + len);
			char* end = vec_at(&group->_raw, old_size);
			snprintf(end, len, "%ld", num_i);
			stringview_nset(sv, end, len);

			/* TODO: Ideal version would just store this in
			 *       binary. The following lines will do that.
			 */
			//vec_append(&group->_raw, &num_i, sizeof(long));
			//sv->data = (char*)vec_end(&group->_raw) - sizeof(long);
			//sv->len = sizeof(long);
			break;
		 }
		case FIELD_FLOAT:
		 {
			double num_f = 0;
			if (column_get_float(&num_f, cols[i], recs)) {
				return FQL_FAIL;
			}
			size_t old_size = group->_raw.size;
			unsigned len = snprintf(NULL, 0, "%f", num_f);
			vec_resize(&group->_raw, old_size + len);
			char* end = vec_at(&group->_raw, old_size);
			snprintf(end, len, "%f", num_f);
			stringview_nset(sv, end, len);
			//vec_append(&group->_raw, &num_f, sizeof(double));
			//sv->data = (char*)vec_end(&group->_raw) - sizeof(double);
			//sv->len = sizeof(double);
		 }
		default:
			;
		}

		/* NOTE: running size is incremented first, because it should
		 *       mark the beginning of the next element. _indicies.size
		 *       should always be number of elements + 1.
		 */
		running_size += sv->len;
		++sv;

		vec_push_back(&group->_indicies, &running_size);
	}

	unsigned group_count = group->val_map.values.size;
	unsigned* idx_ptr = compositemap_get(&group->val_map, &group->_composite);
	int ret = 0;
	if (idx_ptr == NULL) {
		compositemap_set(&group->val_map, 
				 &group->_composite, 
				 &group_count);
		_add_agg_result(group, recs);
		ret = 1;
	} else {
		vec_resize(&group->_raw, raw_size);
		vec_resize(&group->_indicies, index_count);
		_update_agg_result(group, recs, *idx_ptr);
	}

	return ret;
}

/* TODO: eliminate switch with a function pointer to
 *       a function that dumps the aggregate results.
 */
void _read_aggregate(Column* agg_col, String* raw, StringView* sv, size_t idx)
{
	Aggregate* agg = agg_col->field.agg;
	struct aggresult* result = vec_at(&agg->results, idx);
	switch (agg->data_type) {
	case FIELD_INT:
		string_sprintf(raw, "%ld", result->data.i);
		stringview_set_string(sv, raw);
		break;
	case FIELD_FLOAT:
		string_sprintf(raw, "%f", result->data.f);
		stringview_set_string(sv, raw);
		break;
	case FIELD_STRING:
		stringview_set_string(sv, &result->data.s);
	default:
		;
	}
}

int group_dump_record(Group* group, Record* rec)
{
	if (group->_dump_idx >= group->val_map.values.size) {
		return -1;
	}

	unsigned comp_count = group->_composite.size;
	size_t* idx = vec_at(&group->_indicies, comp_count * group->_dump_idx);
	
	int i = 0;
	Column** group_cols = vec_begin(&group->columns);
	StringView* rec_svs = vec_begin(rec->fields);
	for (; i < group->columns.size; ++i) {
		if (group_cols[i]->expr == EXPR_AGGREGATE) {
			_read_aggregate(group_cols[i],
					vec_at(rec->_field_data, i),
					&rec_svs[i],
					group->_dump_idx);
			continue;
		}
		stringview_nset(&rec_svs[i],
				vec_at(&group->_raw, *idx),
				*(idx+1) - *idx);
		++idx;
	}

	++group->_dump_idx;
	return FQL_GOOD;
}
