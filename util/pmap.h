#ifndef PMAP_H
#define PMAP_H

#include "hmap.h"
#include "vec.h"

/* lol */
typedef Hmap Pmap;

Pmap* pmap_new(size_t limit, unsigned props);
Pmap* pmap_construct(Pmap*, size_t limit, unsigned props);
void pmap_destroy(Pmap*);
void pmap_free(Pmap*);
struct vec* pmap_nget(Pmap*, const char*, int);
struct vec* pmap_get(Pmap*, const char*);
struct vec* pmap_nset(Pmap*, const char* key, void* data, int);
struct vec* pmap_set(Pmap*, const char* key, void* data);
struct vec* pmap_remove(Pmap*, const char* key);

#endif /* PMAP_H */
