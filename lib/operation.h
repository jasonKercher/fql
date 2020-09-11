#ifndef OPERATION_H
#define OPERATION_H


#ifdef __cplusplus
extern "C" {
#endif

#include "util/queue.h"
#include "table.h"


/** Operation **/
enum oper {        
        OP_NONE,  
        OP_SELECT,
        OP_DELETE,
        OP_UPDATE,
};


#ifdef __cplusplus
}
#endif

#endif /* OPERATION_H */


