/*
 * @file    basename.c
 * @brief   replacement for missing or faulty basename()
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 2001-2013 Benjamin Gerard
 *
 * Time-stamp: <2013-08-03 08:54:25 ben>
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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_BASENAME
# error "compiling basename.c but already have a basename() function"
#endif

#include "libc68.h"

#ifndef PATH_SEP
#  ifdef NATIVE_WIN32
#   define PATH_SEP { '/' , '\\' }
#  else
#   define PATH_SEP { '/' }
#  endif
# endif
#endif


/**
 * @warning  Sloppy replacement. In many cases this function won't
 *           behave as basename() should.
 */
char *basename(char *path)
{
  static const char sep[] = PATHSEP;
  int i,c,j,k;
  for (i=j=0; c = path[i], c; ++i)
    for (k=0; k < sizeof(sep); ++k)
      if (c == sep[k]) {
        j = i+1;
        break;
      }
  return path + j;
}
