/**
 * @ingroup  file68_lib
 * @file     sc68/error68.h
 * @author   Benjamin Gerard
 * @date     2003-08-08
 * @brief    Error message header.
 *
 */

/* $Id$ */

/* Copyright (C) 1998-2009 Benjamin Gerard */

#ifndef _FILE68_ERROR68_H_
#define _FILE68_ERROR68_H_

#ifndef FILE68_API
#include "file68_api.h"
#endif
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup  file68_error  Error message
 *  @ingroup   file68_lib
 *
 *    Provides error message facilities.
 *
 *  @{
 */

FILE68_API
/** Print a formatted error message.
 *
 *    The error68() function prints an error message via
 *    the error68_t handler function previously set with 
 *    error68_set_handler() function. If cookie is non nul
 *    it is used as first parameter for the error68_t handler.
 *    If it is nul the default cookie set with error68_set_cookie()
 *    function is used instead.
 *
 *  @param  cookie   Fisrt parameter for the error68_t handler
 *  @param  format   printf() like format string
 *
 *  @return error-code
 *  @retval -1
 */
int error68(const char * format, ... );

FILE68_API
/** Print a formatted error message (variable argument version).
 *
 *  @see error68()
 */
int error68_va(const char * format, va_list list);

/**
 *  @}
 */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _FILE68_ERROR68_H_ */
