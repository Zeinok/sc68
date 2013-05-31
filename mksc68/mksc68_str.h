/**
 * @ingroup  mksc68_prg
 * @file     mksc68_str.h
 * @author   Benjamin Gerard
 * @date     2009-01-01
 * @brief    Various string functions.
 *
 */

/* Copyright (C) 1998-2013 Benjamin Gerard */

#ifndef _MKSC68_STR_H_
#define _MKSC68_STR_H_

#include "mksc68_def.h"

EXTERN68 int    str_tracklist(const char ** ptr_tl, int * from, int * to);
EXTERN68 int    str_time_stamp(const char ** ptr_tl, int * ms);
EXTERN68 int    str_time_range(const char ** ptr_tl, int * from, int * to);
EXTERN68 char * str_timefmt(char * buf, int len, unsigned int ms);

#endif /* #ifndef _MKSC68_STR_H_ */
