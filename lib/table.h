#ifndef TABLE_H
#define TABLE_H

#include "query.h"
#include "field.h"
#include "schema.h"
#include "fqlimits.h"
#include "util/hashmap.h"
#include "util/stringy.h"

enum source_type {
	SOURCE_TABLE,
	SOURCE_SUBQUERY,
};

struct table {
	string name;
	string alias;
	struct query* subquery;
	struct reader* reader;
	struct schema* schema;
	struct logicgroup* condition;
	struct process* read_proc;
	void* join_data;
	size_t idx;
	enum source_type source_type;
	enum join_type join_type;
	bool must_reopen;
	bool is_stdin;
};
typedef struct table table;

struct table* table_construct(struct table*,
                              char* name,
                              const char* alias,
                              size_t idx,
                              enum join_type);
struct table* table_construct_subquery(struct table*,
                                       struct query*,
                                       const char* alias,
                                       size_t idx,
                                       enum join_type);
void table_destroy(struct table*);
int table_reset(struct table*, bool has_executed);
int table_resolve_schema(struct table*, struct fqlhandle*);

#define HASH_JOIN_MIN_SIZE 128

struct hashjoin {
	multimap* hash_data;
	struct expression* left_expr;
	struct expression* right_expr;
	struct vec* recs;
	enum join_side state;
	enum field_type comp_type;
	unsigned rec_idx;
};
typedef struct hashjoin hashjoin;

struct hashjoin* hashjoin_construct(struct hashjoin*);
void hashjoin_destroy(struct hashjoin*);
void table_hash_join_init(struct table*);

#endif /* TABLE_H */
