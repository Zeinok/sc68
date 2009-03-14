/**
 * @ingroup  debug68_prg
 * @file     debug68_io.h
 * @author   Benjamin Gerard
 * @date     1999/08/14
 * @brief    debug68 fake IO emulation header.
 */

/* $Id: SC68debug_io.h,v 2.2 2003/11/20 21:10:39 benjihan Exp $ */

/* Copyright (C) 1998-2009 Benjamin Gerard */

#ifndef _DEBUG68_IO_H_
#define _DEBUG68_IO_H_

#ifndef DEBUG68_EXTERN
# ifdef __cplusplus
#  define DEBUG68_EXTERN extern "C"
#else
#  define DEBUG68_EXTERN extern
# endif
#endif

#include "emu68/struct68.h"

typedef struct debug68_io_s debug68_io_t;

/** fake IO emulator common data. */
struct debug68_io_s {
  io68_t io;
  int access;
  int68_t ( *  read) (debug68_io_t *, addr68_t);
  void    ( * write) (debug68_io_t *, addr68_t, int68_t);
  int     ( * reset) (debug68_io_t *);
};

DEBUG68_EXTERN debug68_io_t * pl_dio;
DEBUG68_EXTERN debug68_io_t * ym_dio;
DEBUG68_EXTERN debug68_io_t * mw_dio;
DEBUG68_EXTERN debug68_io_t * sh_dio;

DEBUG68_EXTERN
/** Plug all debug IO. */
int debug68_io_plug(emu68_t * emu68);

DEBUG68_EXTERN
/** Unplug all debug IO. */
void debug68_io_unplug(emu68_t * emu68);

#endif /* #ifndef _DEBUG68_IO_H_ */
