/**
 * @ingroup   emu68_devel
 * @file      error68.c
 * @author    Ben(jamin) Gerard
 * @date      1999/03/13
 * @brief     Error message handler
 * @version   $Id$
 */

/* Copyright (C) 1998-2007 Ben(jamin) Gerard */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "error68.h"

#ifdef MAXERROR
#undef MAXERROR
#endif
#ifdef MAXERRORSTR
#undef MAXERRORSTR
#endif

#define MAXERROR    (sizeof(emu68->err)/sizeof(emu68->err[0]))
#define MAXERRORSTR sizeof(emu68->err[0])

/*  Push error message
 */
int emu68_error_add(emu68_t * emu68, char *format, ... )
{
  if (emu68 && format) {
    va_list list;
    int n = emu68->nerr;

    if(n >= MAXERROR) {
      memmove(emu68->err[0],emu68->err[1],MAXERRORSTR*(MAXERROR-1));
      n = MAXERROR-1;
    }

    va_start(list,format);
    vsnprintf(emu68->err[n],MAXERRORSTR,format,list);
    va_end(list);
    emu68->err[n][MAXERRORSTR-1] = 0;
    emu68->nerr = ++n;
  }
  return -1;
}

const char * emu68_error_empty = "no more error";

/*  Pop error message
 */
const char * emu68_error_get(emu68_t * emu68)
{
  const char * res = emu68_error_empty;
  if (emu68 && emu68->nerr > 0) {
    res = emu68->err[--emu68->nerr];
  }
  return res;
}
