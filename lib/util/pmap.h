#ifndef PMAP_H
#define PMAP_H

#include "hmap.h"
#include "vec.h"

/* lol */
typedef Hmap Pmap;

Pmap* pmap_new();
Pmap* pmap_init(Pmap*);
void pmap_free(Pmap*);
struct vec* pmap_get(Pmap*, const char*);
struct vec* pmap_set(Pmap*, char* key, void* data);
struct vec* pmap_remove(Pmap*, const char* key);

#endif /* PMAP_H */
