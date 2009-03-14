/**
 * @ingroup  debug68_prg
 * @file     debug68_error.h
 * @author   Benjamin Gerard
 * @date     2009-02-01
 * @brief    error message header.
 */

/* $Id: debugmsg68.h 32 2009-01-29 05:27:52Z benjihan $ */

/* Copyright (C) 1998-2009 Benjamin Gerard */

#ifndef _DEBUG68_ERROR_H_
#define _DEBUG68_ERROR_H_

#ifndef DEBUG68_EXTERN
# ifdef __cplusplus
#  define DEBUG68_EXTERN extern "C"
#else
#  define DEBUG68_EXTERN extern
# endif
#endif

#include <stdarg.h>

DEBUG68_EXTERN
int debug68_error_add_va(const char * fmt, va_list list);

DEBUG68_EXTERN
int debug68_error_add(const char * fmt, ...);

DEBUG68_EXTERN
const char * debug68_error_get(void * sc68);

#endif /* #ifndef _DEBUG68_ERROR_H_ */
