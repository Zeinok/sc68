/**
 * @ingroup   debug68_prg
 * @file      debug68_debugcom.h
 * @author    Benjamin Gerard
 * @date      1999/07/11
 * @brief     debug68 debugger header.
 */

/* $Id: SC68debug_debugcom.h,v 2.0 2003/08/21 04:58:35 benjihan Exp $ */

/* Copyright (C) 1998-2009 Benjamin Gerard */

#ifndef _SC68DEBUG_DEBUGCOM_H_
#define _SC68DEBUG_DEBUGCOM_H_

#include "emu68/struct68.h"

#ifndef DEBUG68_EXTERN
# ifdef __cplusplus
#  define DEBUG68_EXTERN extern "C"
#else
#  define DEBUG68_EXTERN extern
# endif
#endif

DEBUG68_EXTERN
/** Enter debugger mode.
 */
int debug68_entering(emu68_t *);

DEBUG68_EXTERN
/** Run a new debugger command.
 *
 * @param   na  number of argument, 0:previous command
 * @param   a   array of argument.
 *
 */
int debug68_newcom(int na, char **a);

DEBUG68_EXTERN
/** Display debugger shell prompt as "diskname track desa>".
 *
 *  @param  diskname  Used to build prompt.
 *  @param  track_num Used to build prompt.
 *
 *  @return prompt string.
 *
 * @warning Return pointer to a static buffer.
 */
char * debug68_prompt(char *diskname, int track_num);

#endif /* #ifndef _SC68DEBUG_DEBUGCOM_H_ */
