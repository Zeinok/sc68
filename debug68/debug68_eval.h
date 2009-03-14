/**
 * @ingroup  debug68_prg
 * @file     debug68_eval.h
 * @author   Benjamin Gerard
 * @date     1999/07/12
 * @brief    debug68 expression evaluator.
 */

/* $Id: SC68debug_eval.h,v 2.0 2003/08/21 04:58:35 benjihan Exp $ */

/* Copyright (C) 1998-2009 Benjamin Gerard */

#ifndef _DEBUG68_EVAL_H_
#define _DEBUG68_EVAL_H_

#ifndef DEBUG68_EXTERN
# ifdef __cplusplus
#  define DEBUG68_EXTERN extern "C"
#else
#  define DEBUG68_EXTERN extern
# endif
#endif

#include "emu68/struct68.h"

DEBUG68_EXTERN
/** Evaluate a string 0 or space terminated.
 *
 * @param   s        string to evaluate
 * @param   err_loc  pointer to error location
 */
int debug68_eval(emu68_t * emu68, char *s, char **err_loc);

#endif /* _DEBUG68_EVAL_H_ */
