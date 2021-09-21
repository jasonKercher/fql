#ifndef FLEX_H
#define FLEX_H

/**
 * flex is a FLEXible vector. A flex stores all data
 * in a vector of char. Each element has its own size.
 */

#include "vec.h"
#include "stringview.h"

struct flex {
	struct vec _raw;
	struct vec _pairs;
};
typedef struct flex flex;

struct flex* flex_construct(struct flex* restrict);
void flex_destroy(struct flex* restrict);

bool flex_empty(const struct flex* restrict);
size_t flex_size(const struct flex* restrict);
void* flex_at(const struct flex* restrict, size_t);
struct stringview flex_pair_at(const struct flex* restrict, size_t);
void flex_reserve(struct flex* restrict, size_t);
void flex_resize(struct flex* restrict, size_t);
void flex_clear(struct flex* restrict);
void flex_shink_to_fit(struct flex* restrict);
void flex_pop_back(struct flex* restrict);

void flex_insert(struct flex* restrict,
                 size_t idx,
                 const void* restrict data,
                 unsigned len);
#define flex_insert_(F_, idx_, data_, T_)                                      \
	flex_insert(F_, idx_, data_, sizeof(T_))

void flex_push_back(struct flex* restrict, const void* restrict, unsigned);
#define flex_push_back_(F_, data_, T_) flex_push_back(F_, data_, sizeof(T_))

void flex_push_back_str_int(struct flex* restrict, long);
void flex_push_back_str_float(struct flex* restrict, double);

void flex_remove(struct flex* restrict, size_t);

#endif /* FLEX_H */
