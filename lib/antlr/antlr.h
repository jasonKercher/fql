//#ifndef ANTLR_H
//#define ANLTR_H

#pragma once

#include "../plan.h"

#ifdef __cplusplus
extern "C" {
#endif

int analyze_query(plan_t*, const char*);

#ifdef __cplusplus
}
#endif

//#endif
