/**
 * @ingroup   debug68_prg
 * @file      debug68_breakpoint.h
 * @author    Benjamin Gerard
 * @date      1999/07/14
 * @brief     debug68 breakpoints header.
 */

/* $Id: SC68debug_breakpoint.h,v 2.0 2003/08/21 04:58:35 benjihan Exp $ */

/* Copyright (C) 1998-2009 Benjamin Gerard */

#ifndef _DEBUG68_BREAKPOINT_H_
#define _DEBUG68_BREAKPOINT_H_

#ifndef DEBUG68_EXTERN
# ifdef __cplusplus
#  define DEBUG68_EXTERN extern "C"
#else
#  define DEBUG68_EXTERN extern
# endif
#endif

#include "emu68/type68.h"

DEBUG68_EXTERN
/** Kill and reset breakpoint.
 *
 * @param   addr  Address of breakpoint to kill, ~0 for all
 *
 * @return  breakpoint internal number.
 * @retval  -1:breakpoint does not exist
 */
int debug68_breakp_kill(addr68_t addr);

DEBUG68_EXTERN
/** Display breakpoint info.
 *
 * @param   addr  Address of breakpoint to display, ~0 for all
 * @return  error-code
 * @retval  0:exist
 */
int debug68_breakp_display(addr68_t addr);

DEBUG68_EXTERN
/** Set breakpoint.
 *
 * @param addr    Address of breakpoint to set
 * @param count   # pass before break (see breakpoint_t)
 * @param reset   count reset value after break
 *
 * @return  breakpoint internal number.
 * @retval  -1:breakpoint does not exist
 */
int debug68_breakp_set(addr68_t addr, int count, int reset);

DEBUG68_EXTERN
/** General break test.
 *
 * This function must be call with addr=PC at each instruction emulation.
 *
 * @return internal breakpoint number
 * @retval -1:do not break
 *
 */
int debug68_breakp_test(addr68_t addr);

#endif /*  ifndef _DEBUG68_BREAKPOINT_H_ */
