/**
 * @ingroup  file68_lib
 * @file     sc68/file68_aud.h
 * @author   Benjamin Gerard
 * @date     1998-09-03
 * @brief    audio backend header.
 */
/* Time-stamp: <2013-07-22 00:19:54 ben> */

/* Copyright (C) 1998-2013 Benjamin Gerard */

#ifndef _FILE68_AUD_H_
#define _FILE68_AUD_H_

#include "file68_api.h"

/**
 * @defgroup  file68_aud  Audio output interface.
 * @ingroup   file68_lib
 *
 *   Provides function for controling audio output backend.
 *
 * @{
 */

FILE68_API
/**
 * Set/Get audio output default sampling rate.
 *
 *   Set the default sampling rate for audio output. This default rate
 *   is used by the audio output backend if no sampling rate is
 *   specified.
 *
 * @param   hz  New default sampling rate [8000..96000]; 0:Get current.
 * @return  Either new or current sampling rate. The value may not be
 *          the exact requested value; it depends on the granularity of
 *          the audio backend.
 */
unsigned int audio68_sampling_rate(const unsigned int hz);

/**
 * @}
 */

#endif
