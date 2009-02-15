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
#include "msg68.h"

#include <string.h>

#define error68_feature msg68_ERROR

/* Process error message (printf like format). */
int error68_va(const char * format, va_list list)
{
  if (format) {
    msg68_va(error68_feature,format,list);
  }
  return -1;
}

/* Process error message (printf like format). */
int error68(const char * format, ...)
{
  int err;
  va_list list;
  int len = strlen(format);

  va_start(list, format);
  err = error68_va(format, list);
  if (len > 0 && format[len-1] != '\n') {
    err = error68("\n");
  }
  va_end(list);

  return err;
}
