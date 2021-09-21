#ifndef VARIABLE_H
#define VARIABLE_H

#include "field.h"
#include "query.h"

struct variable {
	enum sql_type type;
	union field value;
	string* _data;
	unsigned limit;
};
typedef struct variable variable;

struct variable* variable_construct(struct variable*);
void variable_destroy(struct variable*);

void variable_clear(struct variable*);
void variable_set_string(struct variable*, const string*);
void variable_set_stringview(struct variable*, const stringview*);
void variable_copy(struct variable*, const struct variable* src);

#endif  /* VARIABLE_H */
