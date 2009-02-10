/**
 * @ingroup   debug68_devel
 * @file      debug68_eval.h
 * @author    Benjamin Gerard
 * @date      1999/07/12
 * @brief     debug68 expression evaluator.
 * @version   $Id: SC68debug_eval.h,v 2.0 2003/08/21 04:58:35 benjihan Exp $
 */

#ifndef _DEBUG68_EVAL_H_
#define _DEBUG68_EVAL_H_

#include "emu68/struct68.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Evaluate a string 0 or space terminated.
 *
 * @param   s        string to evaluate
 * @param   err_loc  pointer to error location
 */
int debug68_eval(emu68_t * emu68, char *s, char **err_loc);

#ifdef __cplusplus
}
#endif

#endif /* _DEBUG68_EVAL_H_ */
