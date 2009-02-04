/**
 * @ingroup  debug68_lib
 * @file     debug68/debug68_error.h
 * @author   Benjamin Gerard
 * @date     2009-02-01
 * @brief    error message header.
 *
 */

/* $Id: debugmsg68.h 32 2009-01-29 05:27:52Z benjihan $ */

/* Copyright (C) 1998-2009 Benjamin Gerard */

#ifndef _DEBUG68_ERROR_H_
#define _DEBUG68_ERROR_H_

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

int debug68_error_add_va(const char * fmt, va_list list);
int debug68_error_add(const char * fmt, ...);
const char * debug68_error_get(void * sc68);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _DEBUG68_ERROR_H_ */
