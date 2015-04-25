/**
 * @ingroup  sourcer68_prg
 * @file     src68_opt.h
 * @author   Benjamin Gerard
 * @date     2015-03-22
 * @brief    command line options.
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef SRC68_OPT_H
#define SRC68_OPT_H

#include "src68_def.h"

typedef struct option opt_t;

void   opt_new(char * shortopts, const opt_t * longopts);
int    opt_get(int argc, char * const argv[], const char * optstring,
               const opt_t * longopts, int * longindex);
int    opt_ind(void);
int    opt_opt(void);
char * opt_arg(void);

#endif
