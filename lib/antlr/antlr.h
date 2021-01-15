//#ifndef ANTLR_H
//#define ANLTR_H

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "fqlhandle.h"

int analyze_query(struct fql_handle*);

#ifdef __cplusplus
}
#endif

//#endif
