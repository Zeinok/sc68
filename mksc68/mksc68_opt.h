/* $Id$ */

#ifndef _MKSC68_OPT_H_
#define _MKSC68_OPT_H_

#include "mksc68_def.h"

#include <getopt.h>
typedef struct option opt_t;

EXTERN68 void opt_create_short(char * shortopts, const opt_t * longopts);
EXTERN68 int  opt_get(int argc, char * const argv[], const char * optstring,
                      const opt_t * longopts, int * longindex);

#endif /* #ifndef _MKSC68_OPT_H_ */
