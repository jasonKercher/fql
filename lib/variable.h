#ifndef VARIABLE_H
#define VARIABLE_H

#include "field.h"
#include "query.h"

struct variable {
	enum sql_type type;
	union field value;
	string* _data;
	string* _import_value;
	unsigned limit;
	bool is_null;
	bool can_be_redeclared;
};
typedef struct variable variable;

struct variable* variable_construct(struct variable*);
void variable_destroy(struct variable*);

void variable_clear(struct variable*);
void variable_reset(struct variable*);
int variable_set_type(struct variable*, enum sql_type);
void variable_set_string(struct variable*, const string*);
void variable_set_stringview(struct variable*, stringview*);
void variable_set_import_value(struct variable*, const char* val);

#endif  /* VARIABLE_H */
