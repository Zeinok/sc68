/**
 * @ingroup  file68_lib
 * @file     sc68/audio68.h
 * @author   Benjamin Gerard
 * @date     1998-09-03
 * @brief    audio backend header.
 *
 */

/* $Id: audio68.h 75 2009-02-04 19:12:14Z benjihan $ */

/* Copyright (C) 1998-2009 Benjamin Gerard */

#ifndef _FILE68_AUDIO68_H_
#define _FILE68_AUDIO68_H_

#ifndef FILE68_API
#include "file68_api.h"
#endif
#include "istream68.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup  file68_audio  Audio backend
 *  @ingroup   file68_lib
 *
 *  @{
 */

FILE68_API
/** Set/Get audio output default sampling rate.
 *
 *  @params  hz  New default sampling rate [8000..96000]; 0:Get cuurent
 *  @return      Current or new default sampling rate
 */
unsigned int audio68_sampling_rate(const unsigned int hz);


/**
 *@}
 */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _FILE68_AUDIO68_H_ */
