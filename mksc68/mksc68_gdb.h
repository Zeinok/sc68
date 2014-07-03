/**
 * @ingroup  mksc68_prg
 * @file     mksc68/mksc68_gdb.h
 * @author   Benjamin Gerard
 * @date     2013-07-09
 * @brief    gdb server function
 */

/* Copyright (c) 1998-2014 Benjamin Gerard */

#ifndef MKSC68_GDB_H
#define MKSC68_GDB_H

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

#define DEFSIG(NAME,VAL,X,Y) SIGVAL_#NAME = VAL

enum {
  SIGVAL_0       = 0,  /* Signal 0 */
  SIGVAL_HUP     = 1,  /* Hangup */
  SIGVAL_INT     = 2,  /* Interrupt */
  SIGVAL_QUIT    = 3,  /* Quit */
  SIGVAL_ILL     = 4,  /* Illegal instruction */
  SIGVAL_TRAP    = 5,  /* Trace/breakpoint trap */
  SIGVAL_ABRT    = 6,  /* Aborted */
  SIGVAL_EMT     = 7,  /* Emulation trap */
  SIGVAL_FPE     = 8,  /* Arithmetic exception */
  SIGVAL_KILL    = 9,  /* Killed */
  SIGVAL_BUS     = 10, /* Bus error */
  SIGVAL_SEGV    = 11, /* Segmentation fault */
  SIGVAL_SYS     = 12, /* Bad system call */
  SIGVAL_PIPE    = 13, /* Broken pipe */
  SIGVAL_ALRM    = 14, /* Alarm clock */
  SIGVAL_TERM    = 15, /* Terminated */
  SIGVAL_URG     = 16, /* Urgent I/O condition */
  SIGVAL_STOP    = 17, /* Stopped (signal) */
  SIGVAL_TSTP    = 18, /* Stopped (user) */
  SIGVAL_CONT    = 19, /* Continued */
  SIGVAL_CHLD    = 20, /* Child status changed */
  SIGVAL_TTIN    = 21, /* Stopped (tty input) */
  SIGVAL_TTOU    = 22, /* Stopped (tty output) */
  SIGVAL_IO      = 23, /* I/O possible */
  SIGVAL_XCPU    = 24, /* CPU time limit exceeded */
  SIGVAL_XFSZ    = 25, /* File size limit exceeded */
  SIGVAL_VTALRM  = 26, /* Virtual timer expired */
  SIGVAL_PROF    = 27, /* Profiling timer expired */
  SIGVAL_WINCH   = 28, /* Window size changed */
  SIGVAL_LOST    = 29, /* Resource lost */
  SIGVAL_USR1    = 30, /* User defined signal 1 */
  SIGVAL_USR2    = 31, /* User defined signal 2 */
  SIGVAL_PWR     = 32, /* Power fail/restart */
  SIGVAL_POLL    = 33, /* Pollable event occurred */
  SIGVAL_WIND    = 34, /* WIND */
  SIGVAL_PHONE   = 35, /* PHONE */
  SIGVAL_WAITING = 36, /* Process's LWPs are blocked */
  SIGVAL_LWP     = 37, /* Signal LWP */
  SIGVAL_DANGER  = 38, /* Swap space dangerously low */
  SIGVAL_GRANT   = 39, /* Monitor mode granted */
  SIGVAL_RETRACT = 40, /*  */
  SIGVAL_MSG     = 41, /* Monitor mode data available */
  SIGVAL_SOUND   = 42, /* Sound completed */
  SIGVAL_SAK     = 43, /* Secure attention */
  SIGVAL_PRIO    = 44, /* PRIO */
};

EXTERN68 gdb_t * gdb_create(void);
EXTERN68 void    gdb_destroy(gdb_t * gdb);
EXTERN68 int     gdb_event(gdb_t * gdb, int vector, void * emu68);
EXTERN68 int     gdb_error(gdb_t * gdb, const char ** errp);
EXTERN68 int     gdb_conf(char * uri);
EXTERN68 void    gdb_get_conf(unsigned char * addr, int * port);

#endif
