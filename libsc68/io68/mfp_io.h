/**
 * @ingroup   io68_mfp_devel
 * @file      io68/mfp_io.h
 * @author    Benjamin Gerard
 * @date      1999/03/20
 * @brief     MFP-68901 IO plugin header.
 *
 * $Id$
 */

/* Copyright (C) 1998-2007 Benjamin Gerard */

#ifndef _IO68_MFP_IO_H_
#define _IO68_MFP_IO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <emu68/struct68.h>

/** @addtogroup  io68_mfp_devel
 *  @{
 */

/** @name MFP-68901 (Atari-ST timers) IO plugin
 *  @{
 */

/** Initialize MFP IO library. */
int mfpio_init(void);

/** Shutdown MFP IO library. */
void mfpio_shutdown(void);

/** MFP-68901 IO plugin instance. */
io68_t * mfpio_create(emu68_t * const emu68);

/**@}*/

/**
 *@}
 */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _IO68_MFP_IO_H_ */
