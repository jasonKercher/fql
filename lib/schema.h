#ifndef SCHEMA_H
#define SCHEMA_H

#include "io.h"
#include "fqlimits.h"
#include "fqlhandle.h"
#include "util/hashmap.h"
#include "util/stringy.h"

struct schema {
	struct vec* expressions;
	multimap* expr_map;
	string* schema_path;
	string* name;
	char delimiter[DELIM_LEN_MAX];
	char rec_terminator[DELIM_LEN_MAX];
	int strictness;
	enum io io_type;
	enum io write_io_type;
	bool is_default;
	bool delim_is_set;
};
typedef struct schema schema;

enum join_side {
	SIDE_UNDEF,
	SIDE_LEFT,
	SIDE_RIGHT,
	SIDE_MIXED,
};

struct expression;
struct query;

struct schema* schema_construct(struct schema*);
void schema_destroy(void*);

void schema_set_delim(struct schema*, const char*);

bool schema_eq(const struct schema*, const struct schema*);

int schema_resolve(struct fql_handle*);

void schema_add_expression(struct schema*, struct expression*, int src_idx);
void schema_apply_expression_alias(struct schema* schema, const char* alias);
void schema_preflight(struct schema*);

#endif /* SCHEMA_H */
