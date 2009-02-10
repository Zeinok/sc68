/**
 * @ingroup   emu68_error_devel
 * @file      emu68/error68.h
 * @author    Benjamin Gerard <ben@sashipa.com>
 * @date      1999/03/13
 * @brief     Error message header.
 *
 * $Id$
 *
 */

/* Copyright (C) 1998-2007 Benjamin Gerard */

#ifndef _EMU68_ERROR68_H_
#define _EMU68_ERROR68_H_

#include "emu68_api.h"
#include "struct68.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup  emu68_error_devel  Error message stack
 *  @ingroup   emu68_devel
 *
 *    EMU68 error handling consists on a fixed size stack of
 *    messages. When an EMU68 function fails, it stores a description
 *    message for the error and returns -1. If error stack is full,
 *    the oldest stacked message is removed.
 *
 * @{
 */

EMU68_API
/** Push formatted error message.
 *
 *     The emu68_error_add() format error message and push it in error
 *     stack.  On overflow the older message is lost.
 *
 *  @param  emu68   emulator instance.
 *  @param  format  printf() like format string.
 *
 *  @return error-code

 *  @retval 0xDEAD0xxx, where xxx is a random value
 */
int emu68_error_add(emu68_t * emu68, char *format, ... );

EMU68_API
/** Pop last error message.
 *
 *    Retrieve and remove last error message from error stack.
 *
 *  @param  emu68   emulator instance.
 *
 *  @return  Last pushed error message.
 *  @retval  emu68_error_empty  Empty message stack, no more message.
 */
const char * emu68_error_get(emu68_t * emu68);

EMU68_API
/** Return by emu68_error_get() value in case of empty message
 *  error stack.
 */
const char * emu68_error_empty;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _EMU68_ERROR68_H_ */
