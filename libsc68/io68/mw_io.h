/**
 * @ingroup   io68_mw_devel
 * @file      io68/mw_io.h
 * @author    Benjamin Gerard
 * @date      1999/03/20
 * @brief     STE sound IO plugin header.
 *
 */

/* $Id$ */

/* Copyright (C) 1998-2009 Benjamin Gerard */

#ifndef _IO68_MW_IO_H_
#define _IO68_MW_IO_H_

#include "io68_api.h"
#include "mwemul.h"

/** @addtogroup  io68_mw_devel
 *  @{
 */

/** @name MicroWire/LMC (STE sound) IO plugin.
 *  @{
 */

IO68_EXTERN
/** Init MW library. */
int mwio_init(mw_parms_t * const parms);

IO68_EXTERN
/** Shutdown MW library. */
void mwio_shutdown(void);

IO68_EXTERN
/** Create MicroWire IO plugin instance. */
io68_t * mwio_create(emu68_t * emu68, mw_parms_t * const parms);

IO68_EXTERN
/** Get/Set sampling rate.
 *
 * @param  io  MW IO instance
 * @param  hz  0:read current sampling rate, >0:new requested sampling rate
 *
 * @return actual sampling rate
 *
 */
uint68_t mwio_sampling_rate(io68_t * const io, uint68_t sampling_rate);

IO68_EXTERN
/** Get MW emulator instance. */
mw_t * mwio_emulator(io68_t * const io);

/** @} */

/**
 *  @}
 */

#endif /* #ifndef _IO68_MW_IO_H_ */
