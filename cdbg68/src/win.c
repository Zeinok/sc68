
/*
 *                        cdbg68 - sc68 debugger
 *
 *            Copyright (C) 2001-2011 Ben(jamin) Gerard
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
#include "win.h"

int win_is_root(win_t * win) 
{
  return win->root == 0;
}

void win_


/* Default callbaks */

void windef_getbox(win_t * win, box_t * box)
{
  box->x = win->win->start_x;
  box->y = win->win->start_y;
  box->w = win->win->width;
  box->h = win->win->height;
}

void windef_setbox(win_t * win, box_t * box)
{
  int old_x = win->win->start_x;
  int old_y = win->win->start_y;
  int old_w = win->win->width;
  int old_h = win->win->height;

  if ( box->w != old_w || box->h != old_h ) {
    wresize(win->win, box->h, box->w);
  }
  if ( box->x != old_x || box->y != old_y ) {
    wmove(win->win, box->y, box->x);
  }

  box->x = win->win->start_x;
  box->y = win->win->start_y;
  box->w = win->win->width;
  box->h = win->win->height;
}
