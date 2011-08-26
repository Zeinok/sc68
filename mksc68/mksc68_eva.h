/* $Id: SC68debug_eval.h,v 2.0 2003/08/21 04:58:35 benjihan Exp $ */

/* Copyright (C) 1998-2009 Benjamin Gerard */

#ifndef _MKSC68_EVA_H_
#define _MKSC68_EVA_H_

#include "mksc68_def.h"

EXTERN68
/** Evaluate a string 0 or space terminated.
 *
 * @param   expr     expression string to evaluate.
 * @param   status   pointer to status.
 * @return           result of expression.
 */
int eval(char * expr, int * status);

#endif /* _MKSC68_EVA_H_ */
