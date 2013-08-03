/**
 * @ingroup  libc68_lib
 * @file     libc68.h
 * @author   Benjamin Gerard
 * @date     2013-08-02
 * @brief    Libc missing or faulty functions replacements.
 */
/* Time-stamp: <2013-08-03 08:35:30 ben> */

/* Copyright (C) 1998-2013 Benjamin Gerard */

#ifndef LIBC68_H
#define LIBC68_H

#ifndef SC68_EXTERN
# ifdef __cplusplus
#  define SC68_EXTERN extern "C"
# else
#  define SC68_EXTERN
# endif
#endif

SC68_EXTERN
unsigned long int strtoul(const char *s, char **end, int base);

SC68_EXTERN
char *basename(char *path);

#endif
