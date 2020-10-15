#ifndef PMAP_H
#define PMAP_H

#include "hmap.h"
#include "vec.h"

/* lol */
#define pmap hmap

struct pmap* pmap_new();
void pmap_free(struct pmap*);
struct vec* pmap_get(struct pmap*, const char*);
struct vec* pmap_set(struct pmap*, char* key, void* data);
struct vec* pmap_remove(struct pmap*, const char* key);


#endif
