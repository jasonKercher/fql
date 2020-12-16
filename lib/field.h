#ifndef FIELD_H
#define FIELD_H

#include "util/fqlstring.h"

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

#endif  /* FIELD_H */
