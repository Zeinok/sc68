/**
 * @ingroup   mksc68_prg
 * @file      mksc68_msg.h
 * @author    Benjamin Gerard
 * @date      2011/11/01
 * @brief     message output header.
 */

/* Copyright (C) 1998-2011 Benjamin Gerard */

#ifndef _MKSC68_MSG_H_
#define _MKSC68_MSG_H_

#include <stdarg.h>

#include "mksc68_def.h"

EXTERN68 void msgdbg_va(const char * fmt, va_list list);
EXTERN68 void msgdbg(const char * fmt, ...);

EXTERN68 void msgwrn_va(const char * fmt, va_list list);
EXTERN68 void msgwrn(const char * fmt, ...);

EXTERN68 void msgerr_va(const char * fmt, va_list list);
EXTERN68 void msgerr(const char * fmt, ...);

EXTERN68 void msginf_va(const char * fmt, va_list list);
EXTERN68 void msginf(const char * fmt, ...);

EXTERN68 void msgnot_va(const char * fmt, va_list list);
EXTERN68 void msgnot(const char * fmt, ...);

EXTERN68
void msg_callback(const int bit, void *data, const char *fmt, va_list list);

EXTERN68 void msg_init(int level);

#endif /* ifndef _MKSC68_MSG_H_ */
