/*
 * @file    wasc68.h
 * @brief   sc68-ng plugin for winamp 5.5 - defines and declares
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 1998-2014 Benjamin Gerard
 *
 * Time-stamp: <2014-02-07 19:59:57 ben>
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

#ifndef WASC68_H
#define WASC68_H

#ifndef NOVTABLE
# define NOVTABLE
#endif

#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif

#ifndef _WIN32_IE
# define _WIN32_IE 0x0400                /* for UDM_SETRANGE32 */
#endif

#ifndef EXTERN
# ifdef __cplusplus
#  define EXTERN extern "C"
# else
#  define EXTERN extern
# endif
#endif

#ifndef EXPORT
# define EXPORT EXTERN __declspec(dllexport)
#endif

#ifdef __GNUC__
# define FMT12 __attribute__ ((format (printf, 1, 2)))
#else
# define FMT12
#endif

/* in_sc68.c */
EXTERN void set_asid(int asid);

/* dll.c */
EXTERN int g_useufi, g_usehook;

/* dbg.c */
#include <stdarg.h>
EXTERN void dbg(const char *, ...) FMT12;
EXTERN void dbg_va(const char *, va_list);
EXTERN void msgfct(const int, void *, const char *, va_list);

#ifndef DBG
# if defined(DEBUG) || !defined(NDEBUG)
#  define DBG(FMT,...) dbg("%s -- " FMT, __FUNCTION__, ## __VA_ARGS__)
# else
#  define DBG(FMT,...) if(0);else
# endif
#endif

/* tracksel.c */
enum {
  REMEMBER_NOT_SET    = -666,
  REMEMBER_ALL_TRACKS = -1,
  REMEMBER_DEF_TRACK  =  0,
};

/* cache.c */
EXTERN int   wasc68_cache_init(void);
EXTERN void  wasc68_cache_kill(void);
EXTERN void* wasc68_cache_get(const char * uri);
EXTERN void  wasc68_cache_release(void * disk, int dont_keep);

#endif
