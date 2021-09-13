#ifndef FQLPLAN_H
#define FQLPLAN_H

#include "util/node.h"
#include "util/dgraph.h"

struct query;
struct fql_handle;

struct fql_plan {
	struct dgraph* processes;
	struct fql_handle* fql_ref;
	struct dnode* op_true;
	struct dnode* op_false;
	struct dnode* current; /* temp */
	struct vec* execution_vector;
	struct vec* _root_data;
	struct fifo* global_root;
	struct vec* root_fifo_vec;
	struct query* query;
	size_t rows_affected;
	size_t iterations;
	int source_count;
	int plan_id;
	int pipe_factor;
	bool is_const;
	bool has_stepped;
	bool loose_groups;
};
typedef struct fql_plan plan;

struct fql_plan* plan_construct(struct fql_plan*, struct query*, struct fql_handle*);
void plan_destroy(void*);

int build_plans(struct fql_handle*);
void print_plans(struct fql_handle*);

#endif /* FQLPLAN_H */
