#include "group.h"
#include "column.h"
#include "function.h"
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
	*group = (Group) {
		 vec_new_(Column*)            /* columns */
		,vec_new_(Aggregate)          /* aggregates */
		,hashmap_new_(unsigned, 128,  /* groups */
			HASHMAP_PROP_NOCASE | HASHMAP_PROP_RTRIM)
		,{ 0 }                        /* composite */
	};

	vec_construct_(&group->_composite, StringView);

	return group;
}

void group_free(Group* group)
{
	group_destroy(group);
	free_(group);
}
void group_destroy(Group* group)
{
	vec_free(group->columns);
	vec_free(group->aggregates);
	hashmap_free(group->groups);
	vec_destroy(&group->_composite);
}

void group_add_column(Group* group, Column* col)
{
	vec_push_back(group->columns, &col);
	vec_add_one(&group->_composite);
}

void group_cat_description(Group* group, Process* proc)
{
	Column** it = vec_begin(group->columns);
	for (; it != vec_end(group->columns); ++it) {
		if (it != vec_begin(group->columns)) {
			string_strcat(proc->action_msg, ",");
		}
		column_cat_description(*it, proc->action_msg);
	}
}

