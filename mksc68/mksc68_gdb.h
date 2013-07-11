/**
 * @ingroup  mksc68_prg
 * @file     mksc68/mksc68_gdb.h
 * @author   Benjamin Gerard
 * @date     2013-07-09
 * @brief    gdb server function
 */
/* Time-stamp: <2013-07-11 19:46:04 ben> */

/* Copyright (C) 1998-2013 Benjamin Gerard */

#ifndef _MKSC68_GDB_H_
#define _MKSC68_GDB_H_

#include "mksc68_def.h"

typedef struct gdb_s gdb_t;

enum {
  RUN_IDLE,                         /* waiting for gdb              */
  RUN_STOP,                         /* just stopped (breakpoint...) */
  RUN_CONT,                         /* running (stepping or normal) */
  RUN_EXIT,                         /* exit requested               */
  RUN_SKIP,                         /* detached / ignore            */
};

enum {
  CODE_IDLE,
  CODE_ERROR,
  CODE_KILL,
  CODE_DETACH,
  CODE_TRACE,
  CODE_STOP,
  CODE_BREAK,
  CODE_HALT,
  CODE_INT,
  CODE_PRIVATE,
};


EXTERN68 gdb_t * gdb_create(void);
EXTERN68 void    gdb_destroy(gdb_t * gdb);
EXTERN68 int     gdb_event(gdb_t * gdb, int vector, void * emu68);
EXTERN68 int     gdb_error(gdb_t * gdb, const char ** errp);
EXTERN68 int     gdb_conf(char * uri);
EXTERN68 void    gdb_get_conf(unsigned char * addr, int * port);

#endif /* ifndef _MKSC68_GDB_H_ */
