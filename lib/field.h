#ifndef FIELD_H
#define FIELD_H

#include "util/fqlstring.h"
#include "util/stringview.h"

/** data type **/
enum field_type {
        FIELD_UNDEFINED,
        FIELD_STRING,
        FIELD_INT,
        FIELD_FLOAT,
};

/* forward declaration */
struct function;

union field {
        struct function* fn;
        String* s;
        double f;
        long i;
}; 

int field_to_int(long*, union field*, enum field_type*);
int field_to_float(double*, union field*, enum field_type*);
int field_to_stringview(struct stringview*, union field*, enum field_type*);

#endif  /* FIELD_H */
