#ifndef TABLE_H
#define TABLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "util/stack.h"
#include "util/hashmap.h"
//#include "util/pmap.h"
#include "util/stringy.h"
#include "schema.h"
//#include "reader.h"
#include "fqlimits.h"

//struct table {
//        struct reader* reader;
//        struct schema* schema;
//        string name;
//};

/** source **/
enum source_type {
	SOURCE_TABLE,
	SOURCE_SUBQUERY,
};

enum join_type {
	JOIN_FROM,
	JOIN_INNER,
	JOIN_LEFT,
	JOIN_RIGHT,
	JOIN_FULL,
	JOIN_CROSS,
};

struct table {
	string name;
	string alias;
	struct query* subquery;
	struct reader* reader;
	struct schema* schema;
	struct logicgroup* condition;
	struct vec* validation_list;
	struct process* read_proc;
	void* join_data;
	size_t idx;
	enum source_type source_type;
	enum join_type join_type;
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
char* table_get_delim(struct table*);

#define HASH_JOIN_MIN_SIZE 128

struct hashjoin {
	struct hashmap hash_data;
	struct column* left_col;
	struct column* right_col;
	vec* recs;
	enum join_side state;
	unsigned rec_idx;
};
typedef struct hashjoin hashjoin;

struct hashjoin* hashjoin_construct(struct hashjoin*);
void hashjoin_destroy(struct hashjoin*);
void table_hash_join_init(struct table*);


#ifdef __cplusplus
}
#endif

#endif /* TABLE_H */


