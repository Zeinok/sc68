/**
 * @ingroup   emu68_core_devel
 * @file      emu68/cc68.h
 * @author    Benjamin Gerard
 * @date      1999/03/13
 * @brief     Condition code function table header.
 *
 * $Id$
 *
 */

/* Copyright (C) 1998-2007 Benjamin Gerard */

#ifndef _EMU68_CC68_H_
#define _EMU68_CC68_H_

#include "type68.h"

/* $$$ TEST for mingw32 bug: table is uninitialized and belongs to BSS
   section instead of ronly_data or something like that.
	 
   $$$ TEST success !!! It worls with this extern here.... Damn There
   is probably a bunch of similar bugs in the whole code.
*/

#ifdef __cplusplus
#define DECLTABLE extern "C"
#else
#define DECLTABLE extern
#endif



/** @addtogroup   emu68_core_devel
 *  @{
 */

DECLTABLE
/** Code condition testing function table.
 *
 *    Condition code function table is used by EMU68 for conditional
 *    instructions emulation including Bcc, Scc and DCcc. The 4
 *    condition bits of instruction is used as index to call
 *    corresponding test function. Each test function performs
 *    suitable SR bits operations and return 0 if condition is false
 *    and other if condition is satisfied.
 */
int (* const is_cc68[])(int);

/**
 *@}
 */

#endif /* #ifndef _EMU68_CC68_H_ */
