/*
 *                 file68 - Error message handling
 *	      Copyright (C) 2001-2009 Ben(jamin) Gerard
 *	     <benjihan -4t- users.sourceforge -d0t- net>
 *
 * This  program is  free  software: you  can  redistribute it  and/or
 * modify  it under the  terms of  the GNU  General Public  License as
 * published by the Free Software  Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
 * MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
 * General Public License for more details.
 *
 * You should have  received a copy of the  GNU General Public License
 * along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

/* $Id$ */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include "error68.h"
#include "debugmsg68.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

const int error68_feature = debugmsg68_ERROR;

/* Default cookie. */
static void * default_cookie = 0;

/* Default error handler prints to stderr and adds an additionnal '\n'
 * if neccessary.
 */
static void default_handler(void * cookie, const char * fmt, va_list list)
{
  FILE * out = cookie?(FILE*)cookie:stderr;
  int l = strlen(fmt)-1;
  vfprintf(out,fmt,list);
  if (l>=0 && fmt[l] != '\n') {
    fputc('\n',out);
  }
}

/* Error handler. */
static error68_t error68_fct = default_handler;

/* Set error handler. */
error68_t error68_set_handler(error68_t handler)
{
  error68_t old = error68_fct;
  error68_fct = handler;
  return old;
}

/* Set error default cookie. */
void * error68_set_cookie(void * cookie)
{
  void * old = default_cookie;
  default_cookie = cookie;
  return old;
}

/* Process error message (printf like format). */
int error68_va(void * cookie, const char * format, va_list list)
{
  if (error68_fct && format) {
    debugmsg68(error68_feature,"ERROR::");
    vdebugmsg68(error68_feature,format,list);
    error68_fct(cookie?cookie:default_cookie, format, list);
  }
  return -1;
}

/* Process error message (printf like format). */
int error68(void * cookie, const char * format, ...)
{
  va_list list;
  va_start(list, format);
  return error68_va(cookie, format, list);
}
