#include "fqldelete.h"
#include "query.h"
#include "fql.h"

int _delete_no_op(fqldelete*, node*);

fqldelete* fqldelete_construct(fqldelete* self)
{
	*self = (fqldelete) {
		OP_DELETE,
		NULL,
		NULL,
		&_delete_no_op,
		0,
		-1,
	};

	return self;
}

void fqldelete_destroy(fqldelete* self)
{
	
}

void fqldelete_apply_process(query* query, plan* plan)
{

}

void fqldelete_preop(fqldelete* self, query* query)
{
}

int _delete_no_op(fqldelete* self, node* rg)
{
	return FQL_GOOD;
}

