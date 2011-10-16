/* $Id$ */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "mksc68_msg.h"
#include "mksc68_cmd.h"

#include <stdio.h>
#include <sc68/msg68.h>

static int cat = msg68_DEFAULT;

void msgdbg_va(const char * fmt, va_list list)
{
  msg68_va(cat, fmt, list);
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

void msg_callback(const int bit, void *data, const char *fmt, va_list list)
{
  /* select output: always error output except for INFO messages */
  FILE * out = ( bit == msg68_INFO )
    ? stdout
    : stderr
    ;
  if (bit < msg68_INFO)
    fprintf(out,"%s: ", cmd_cur());
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

static int mksc68_cat = msg68_DEFAULT;

void msg_init(int level)
{
  level += msg68_INFO;
  if (level < msg68_CRITICAL) level = msg68_CRITICAL;
  if (level > msg68_TRACE)    level = msg68_TRACE;
  msg68_cat_level(level);
  mksc68_cat = msg68_cat("mksc68","sc68 maker",MKSC68_O);
  if (mksc68_cat == -1) mksc68_cat = msg68_DEFAULT;
}
