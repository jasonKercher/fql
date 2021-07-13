#ifndef ORDER_H
#define ORDER_H

#include <stdio.h>
#include "record.h"
#include "util/vec.h"
#include "util/flex.h"

struct query;
struct expression;
struct process;
struct fqlselect;
struct order;
struct _entry;

typedef int (*order_select_fn)(struct order*, struct process*);

struct order {
	struct vec expressions;
	struct vec entries;
	struct flex order_data;
	order_select_fn select__;
	struct vec* api;
	const char* in_filename;
	FILE* out_file;
	struct _entry* entry_iter;
	char* mmap;
	size_t file_size;
	int fd;
	bool sorted;
};
typedef struct order order;

struct order* order_construct(struct order*);
void order_destroy(struct order*);

int order_add_expression(struct order*, struct expression*);
int order_init_io(struct order*, const char* in, const char* out);
void order_cat_description(struct order*, struct process*);
void order_connect_api(struct query*, struct vec*);
int order_preresolve_expressions(struct order*, struct fqlselect*);
int order_add_record(struct order*, struct recgroup*);
int order_sort(struct order*);

#endif /* ORDER_H */
