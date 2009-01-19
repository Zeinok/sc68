/**
 * @ingroup  file68_istream68
 * @file     file68/istream68_pa.h
 * @author   Benjamin Gerard
 * @date     2007-03-08
 * @brief    PA stream header.
 *
 */

/* $Id$ */

/* Copyright (C) 1998-2009 Benjamin Gerard */

#ifndef _FILE68_ISTREAM68_PA_H_
#define _FILE68_ISTREAM68_PA_H_

#include "istream68.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @name      PortAudio stream
 *  @ingroup   file68_istream68
 *
 *    Implements istream68_t for libportaudio (audio input/output).
 *
 *  @{
 */

/** Creates a stream for libportaudio.
 *
 *  @param  fname    path of file.
 *  @param  mode     bit#0 : read access, bit#1 : write access.
 *
 *  @return stream
 *  @retval 0 on error
 *
 *  @note    filename is internally copied.
 *  @warning write mode only.
 */
istream68_t * istream68_pa_create(const char * fname, int mode);

/**
 *@}
 */

#ifdef __cplusplus
}
#endif

#endif /* #define _FILE68_ISTREAM68_PA_H_ */
