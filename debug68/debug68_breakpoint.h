/**
 * @ingroup   debug68_devel
 * @file      debug68_breakpoint.h
 * @author    Benjamin Gerard
 * @date      1999/07/14
 * @brief     debug68 breakpoints
 * @version   $Id: SC68debug_breakpoint.h,v 2.0 2003/08/21 04:58:35 benjihan Exp $
 */

#ifndef _DEBUG68_BREAKPOINT_H_
#define _DEBUG68_BREAKPOINT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "emu68/type68.h"

/** Kill and reset breakpoint.
 *
 * @param   addr  Address of breakpoint to kill, ~0 for all
 *
 * @return  breakpoint internal number.
 * @retval  -1:breakpoint does not exist
 */
int debug68_breakp_kill(addr68_t addr);

/** Display breakpoint info.
 *
 * @param   addr  Address of breakpoint to display, ~0 for all
 * @return  error-code
 * @retval  0:exist
 */
int debug68_breakp_display(addr68_t addr);

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

/** General break test.
 *
 * This function must be call with addr=PC at each instruction emulation.
 *
 * @return internal breakpoint number
 * @retval -1:do not break
 *
 */
int debug68_breakp_test(addr68_t addr);

#ifdef __cplusplus
}
#endif

#endif /*  ifndef _DEBUG68_BREAKPOINT_H_ */
