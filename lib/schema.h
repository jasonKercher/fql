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
	char delimiter[DELIM_LEN_MAX + 1];
	char rec_terminator[DELIM_LEN_MAX + 1];
	int strictness;
	enum io io_type;
	enum io write_io_type;
	bool is_default;
	bool delim_is_set;
	bool is_preresolved;
};
typedef struct schema schema;

enum join_side {
	SIDE_UNDEF,
	SIDE_LEFT,
	SIDE_RIGHT,
	SIDE_MIXED,
};

enum fuzzy_return {
	FUZZY_SUCCESS,
	FUZZY_AMBIGUOUS,
	FUZZY_NOTFOUND,
};

struct expression;
struct query;

struct schema* schema_construct(struct schema*);
void schema_destroy(void*);

void schema_copy(struct schema*, const struct schema*);
void schema_set_delim(struct schema*, const char*);

enum fuzzy_return
schema_fuzzy_resolve_file(string* dest, const string* input, int strictness);
bool schema_eq(const struct schema*, const struct schema*);

int schema_resolve(struct fqlhandle*);

void schema_add_expression(struct schema*, struct expression*, int src_idx);
void schema_apply_expression_alias(struct schema* schema, const char* alias);
void schema_preflight(struct schema*);

#endif /* SCHEMA_H */
