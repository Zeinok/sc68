/**
 * @ingroup   debug68_devel
 * @file      debug68_io.h
 * @author    Ben(jamin) Gerard
 * @date      1999/08/14
 * @brief     debug68 fake IO emulation
 * @version   $Id: SC68debug_io.h,v 2.2 2003/11/20 21:10:39 benjihan Exp $
 */

#ifndef _DEBUG68_IO_H_
#define _DEBUG68_IO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "emu68/struct68.h"

typedef struct debug68_io_s debug68_io_t;

struct debug68_io_s {
  io68_t io;
  int access;
  int68_t ( *  read) (debug68_io_t *, addr68_t);
  void    ( * write) (debug68_io_t *, addr68_t, int68_t);
  int     ( * reset) (debug68_io_t *);
};

extern debug68_io_t * pl_dio;
extern debug68_io_t * ym_dio;
extern debug68_io_t * mw_dio;
extern debug68_io_t * sh_dio;

/** Plug all debug IO. */
int debug68_io_plug(emu68_t * emu68);

/** Unplug all debug IO. */
void debug68_io_unplug(emu68_t * emu68);


#ifdef __cplusplus
}
#endif

#endif /* #ifndef _DEBUG68_IO_H_ */
