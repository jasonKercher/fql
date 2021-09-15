//#ifndef ANTLR_H
//#define ANLTR_H

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct fqlhandle;
int analyze_query(struct fqlhandle*);

#ifdef __cplusplus
}
#endif

//#endif
