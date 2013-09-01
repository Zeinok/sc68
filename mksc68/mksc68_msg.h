/**
 * @ingroup  mksc68_prg
 * @file     mksc68_msg.h
 * @author   Benjamin Gerard
 * @date     2011-11-01
 * @brief    message output header.
 */
/* Time-stamp: <2013-08-28 11:34:13 ben> */

/* Copyright (C) 1998-2013 Benjamin Gerard */

#ifndef MKSC68_MSG_H
#define MKSC68_MSG_H

#include <stdarg.h>

#include "mksc68_def.h"

#ifndef FMT12
# ifdef __GNUC__
#  define FMT12 __attribute__ ((format (printf, 1, 2)))
# else
#  define FMT12
# endif
#endif

EXTERN68 void msgdbg_va(const char * fmt, va_list list);
EXTERN68 void msgdbg(const char * fmt, ...) FMT12;

EXTERN68 void msgwrn_va(const char * fmt, va_list list);
EXTERN68 void msgwrn(const char * fmt, ...) FMT12;

EXTERN68 void msgerr_va(const char * fmt, va_list list);
EXTERN68 void msgerr(const char * fmt, ...) FMT12;

EXTERN68 void msginf_va(const char * fmt, va_list list);
EXTERN68 void msginf(const char * fmt, ...) FMT12;

EXTERN68 void msgnot_va(const char * fmt, va_list list);
EXTERN68 void msgnot(const char * fmt, ...) FMT12;

EXTERN68
void msg_callback(const int bit, void *data, const char *fmt, va_list list);

EXTERN68 void msg_init(int level);

EXTERN68 void msg_lock(void);
EXTERN68 void msg_unlock(void);

#endif
