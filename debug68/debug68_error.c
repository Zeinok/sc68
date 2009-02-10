/*
 *                     debug68 - error messages                      
 *
 *            Copyright (C) 2001-2009 Ben(jamin) Gerard
 *           <benjihan -4t- users.sourceforge -d0t- net>
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
 * $Id: sc68.c 57 2009-01-31 18:24:54Z benjihan $
 */

/* generated config include */
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "debug68_error.h"

#include "sc68/debugmsg68.h"
#include "sc68/sc68.h"

int debug68_error_add_va(const char * fmt, va_list list)
{
  vdebugmsg68(debugmsg68_ERROR, fmt, list);
  return -1;
}

int debug68_error_add(const char * fmt, ...)
{
  va_list list;
  int err;
  va_start(list,fmt);
  vdebugmsg68(debugmsg68_ERROR, fmt, list);
  err = debug68_error_add_va(fmt, list);
  va_end(list);
  return err;
}

const char * debug68_error_get(void * sc68)
{
  return sc68_error_get(sc68);
}
