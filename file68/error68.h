/**
 * @ingroup  file68_lib
 * @file     file68/error68.h
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
# include "file68_api.h"
#endif
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup   file68_error68  Error message
 *  @ingroup    file68_lib
 *
 *    Provides error message facilities.
 *
 *  @{
 */

/** Error handler type. */
typedef void (*error68_t)(void *, const char *, va_list);

/** For backward compatibility.
 *  @deprecated
 */
#define error68_add error68

FILE68_API
/** Set error handler.
 *
 *   @param   handler  New error handler
 *   @return  Previous handler
 */
error68_t error68_set_handler(error68_t handler);

FILE68_API
/** Set default cookie.
 *
 *    The error68_set_cookie() set the default cookie
 *    to be given to the error68_t handler as first
 *    parameter.
 *
 *  @param   cookie   New default cookie for the error68_t handler
 *  @return  Previous cookie
 */
void * error68_set_cookie(void * cookie);

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
int error68(void * cookie, const char * format, ... );

FILE68_API
/** Print a formatted error message (variable argument version).
 *
 *  @see error68()
 */
int error68_va(void * cookie, const char * format, va_list list);

/**
 *@}
 */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _FILE68_ERROR68_H_ */
