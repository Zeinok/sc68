/**
 * @ingroup   emu68_core_devel
 * @file      emu68/cc68.h
 * @author    Benjamin Gerard
 * @date      1999/03/13
 * @brief     Condition code function table header.
 *
 */

/* $Id$ */

/* Copyright (C) 1998-2009 Benjamin Gerard */

#ifndef _EMU68_CC68_H_
#define _EMU68_CC68_H_

#include "emu68_api.h"
#include "type68.h"

/** @addtogroup   emu68_core_devel
 *  @{
 */

EMU68_EXTERN
/** Code condition testing function table.
 *
 *    Condition code function table is used by EMU68 for conditional
 *    instructions emulation including Bcc, Scc and DCcc. The 4
 *    condition bits of instruction is used as index to call
 *    corresponding test function. Each test function performs
 *    suitable SR bits operations and return 0 if condition is false
 *    and other if condition is satisfied.
 */
int (* const is_cc68[8])(int);

/**
 *  @}
 */

#endif /* #ifndef _EMU68_CC68_H_ */
