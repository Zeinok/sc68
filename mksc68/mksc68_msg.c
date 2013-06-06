/*
 * @file    mksc68_msg.c
 * @brief   output functions
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 1998-2013 Benjamin Gerard
 *
 * Time-stamp: <2013-06-05 23:34:38 ben>
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

/* generated config include */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "mksc68_msg.h"
#include "mksc68_cmd.h"

#include <stdio.h>
#include <string.h>
#include <sc68/msg68.h>


static int mksc68_cat = msg68_DEFAULT;

void msgdbg_va(const char * fmt, va_list list)
{
  msg68_va(mksc68_cat, fmt, list);
}

void msgdbg(const char * fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  msgdbg_va(fmt, list);
  va_end(list);
}

void msgwrn_va(const char * fmt, va_list list)
{
  msg68_va(msg68_WARNING, fmt, list);
}

void msgwrn(const char * fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  msgwrn_va(fmt, list);
  va_end(list);
}

void msgerr_va(const char * fmt, va_list list)
{
  msg68_va(msg68_ERROR, fmt, list);
}

void msgerr(const char * fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  msgerr_va(fmt, list);
  va_end(list);
}

void msginf_va(const char * fmt, va_list list)
{
  msg68_va(msg68_INFO, fmt, list);
}

void msginf(const char * fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  msginf_va(fmt, list);
  va_end(list);
}

void msgnot_va(const char * fmt, va_list list)
{
  msg68_va(msg68_NOTICE, fmt, list);
}

void msgnot(const char * fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  msgnot_va(fmt, list);
  va_end(list);
}

extern int is_interactive;

void msg_callback(const int bit, void *data, const char *fmt, va_list list)
{
  FILE * out = stdout;
  if (bit <= msg68_WARNING) {
    const char * cmd = cmd_cur();
    if (!is_interactive)
      out = stderr;
    if (cmd && strcmp(cmd,"error"))
      fprintf(out,"%s: ", cmd);
  }
  assert(fmt);
  vfprintf(out,fmt,list);
}

#ifndef MKSC68_O
# ifdef DEBUG
#  define MKSC68_O 1
# else
#  define MKSC68_O 0
# endif
#endif

void msg_init(int level)
{
  level += msg68_INFO;
  if (level < msg68_CRITICAL) level = msg68_CRITICAL;
  if (level > msg68_TRACE)    level = msg68_TRACE;
  msg68_cat_level(level);
  mksc68_cat = msg68_cat("mksc68","sc68 maker",MKSC68_O);
  if (mksc68_cat == -1) mksc68_cat = msg68_DEFAULT;
}
