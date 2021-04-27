#include "group.h"
#include "column.h"
#include "util/util.h"

Group* group_new()
{
	Group* new_group = NULL;
	malloc_(new_group, sizeof(*new_group));

	return group_construct(new_group);
}

Group* group_construct(Group* group)
{
	*group = (Group) {
		 vec_new_(Column*)    /* columns */
	};
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

