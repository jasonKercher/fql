#ifndef SCHEMA_H
#define SCHEMA_H

#include "io.h"
#include "fqlhandle.h"
#include "fqlimits.h"
#include "util/queue.h"
#include "util/hashmap.h"
#include "util/vec.h"
#include "util/stringy.h"

struct schema {
	struct vec* columns;
	multimap* col_map;
	string* schema_path;
	string* name;
	char delimiter[DELIM_LEN_MAX];
	char rec_terminator[DELIM_LEN_MAX];
	int strictness;
	enum io io_type;
	enum io write_io_type;
	bool is_default;
};
typedef struct schema schema;

enum join_side {
	SIDE_UNDEF,
	SIDE_LEFT,
	SIDE_RIGHT,
	SIDE_MIXED,
};

struct column;
struct query;

struct schema* schema_construct(struct schema*);
void schema_destroy(void*);

bool schema_eq(const struct schema*, const struct schema*);

int schema_resolve(struct fql_handle*);

void schema_add_column(struct schema*, struct column*, int src_idx);
void schema_apply_column_alias(struct schema* schema, const char* alias);
void schema_preflight(struct schema*);

#endif /* SCHEMA_H */
