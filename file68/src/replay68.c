/*
 * @file    replay68.c
 * @brief   built-in external replay rom
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 1998-2013 Benjamin Gerard
 *
 * Time-stamp: <2013-06-08 21:24:42 ben>
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
# include "config.h"
#endif
#include "file68_api.h"
#include "string68.h"

#include "replay.inc.h"

# include <stdlib.h>

static int cmp(const void * pa, const void * pb)
{
  const struct replay
    * a = (const struct replay *) pa,
    * b = (const struct replay *) pb;
  return strcmp68(a->name, b->name);
}

FILE68_API
int replay68_get(const char * name, const void ** data, int * csize, int * dsize)
{
  struct replay s, *r;
  s.name = name;
  r = bsearch(&s, replays, sizeof(replays)/sizeof(*replays),
              sizeof(*replays), cmp);
  if (r) {
    if (data)
      *data = r->data;
    if (csize)
      *csize = r->csize;
    if (dsize)
      *dsize = r->dsize;
  }
  return -!r;
}
