#include "hashmap.h"
#include "util.h"

HashMap* hashmap_new(size_t limit, unsigned props)
{
        HashMap* new_map = NULL;
        malloc_(new_map, sizeof(*new_map));
        return new_map;
}

HashMap* hashmap_construct(HashMap* m, size_t limit, unsigned props)
{
        *m = (HashMap) {
                 NULL   /* data */
                ,{ 0 }  /* _keybuf */
                ,0      /* _keybufhead */
                ,props  /* props */
        };

        malloc_(m->_data, sizeof(void*) * limit);
        vec_construct_(&m->_keybuf, char);
        return m;
}

void hashmap_free(HashMap* m)
{
        hashmap_destroy(m);
        free_(m);
}

void hashmap_destroy(HashMap* m)
{
        free_(m->_data);
        vec_destroy(&m->_keybuf);
}

void* hashmap_nset(HashMap* m, const char* key, void* data, int n)
{
        return NULL;
}

void* hashmap_set(HashMap* m, const char* key, void* data)
{
        return NULL;
}

void* hashmap_nget(HashMap* m, const char* key, int n)
{
        return NULL;
}
void* hashmap_get(HashMap* m, const char* key)
{
        return NULL;
}

