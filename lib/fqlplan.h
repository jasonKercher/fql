#ifndef FQLPLAN_H
#define FQLPLAN_H

#include "util/node.h"
#include "util/dgraph.h"

struct query;
struct fqlhandle;
struct logicgroup;
struct process;

struct fqlplan {
	struct dgraph* processes;
	struct fqlhandle* fql_ref;
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
typedef struct fqlplan plan;

struct fqlplan* plan_construct(struct fqlplan*, struct query*, struct fqlhandle*);
void plan_destroy(void*);

int plan_logic_to_process(struct fqlplan*, struct process*, struct logicgroup*);
int plan_logicgroup_to_process(struct fqlplan*,
                               struct logicgroup*,
                               struct process** ret,
                               bool is_from_hash_join);

int plan_build(struct fqlhandle*);
void plan_print(struct fqlhandle*);

#endif /* FQLPLAN_H */
