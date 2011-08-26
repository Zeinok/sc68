/* $Id$ */

#ifndef _MKSC68_STR_H_
#define _MKSC68_STR_H_

#include "mksc68_def.h"

EXTERN68 int str_tracklist(const char ** ptr_tl, int * from, int * to);
EXTERN68 int str_time_stamp(const char ** ptr_tl, int * ms);
EXTERN68 int str_time_range(const char ** ptr_tl, int * from, int * to);

#endif /* #ifndef _MKSC68_STR_H_ */
