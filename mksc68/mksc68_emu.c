/*
 * @file    mksc68_emu.c
 * @brief   68k and chipsets emulators
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (C) 1998-2013 Benjamin Gerard
 *
 * Time-stamp: <2013-07-08 07:22:40 ben>
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

/* generated config include */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "mksc68_emu.h"
#include "mksc68_msg.h"
#include <sc68/sc68.h>
#include <emu68/emu68.h>
#include <io68/io68.h>

#include <assert.h>
#include <string.h>


sc68_t   * sc68;
emu68_t  * emu68;
io68_t  ** ios68;

extern int sc68_emulators(sc68_t *, emu68_t **, io68_t  ***);

int emu_init(void)
{
  int ret = -1;
  sc68_create_t create68;

  memset(&create68,0,sizeof(create68));
  create68.log2mem     = 19;
  create68.emu68_debug = 1;
  sc68 = sc68_create(&create68);
  if (!sc68)
    goto error;

  sc68_emulators(sc68, &emu68, &ios68);
  msgdbg("emulators: emu68 %p\n", emu68);
  msgdbg("emulators: io68 %p '%s'\n", ios68[0], ios68[0]->name);
  msgdbg("emulators: io68 %p '%s'\n", ios68[1], ios68[1]->name);
  msgdbg("emulators: io68 %p '%s'\n", ios68[2], ios68[2]->name);
  msgdbg("emulators: io68 %p '%s'\n", ios68[3], ios68[3]->name);
  msgdbg("emulators: io68 %p '%s'\n", ios68[4], ios68[4]->name);

  ret = 0;
 error:
  if (ret)
    emu_shutdown();

  return ret;
}

void emu_shutdown(void)
{
  if (sc68) {
    sc68_close(sc68);
    sc68_destroy(sc68);
    sc68 = 0;
  }
}

int emu_load_sc68(const char * url)
{
  /* sc68_disk_t disk; */

  assert(url);
  assert(sc68);

/* $$$ WTH ???
  disk = sc68_load_disk_url(url);
*/

  if (!sc68) {
    return -1;
  }
/*   sc68_close(sc68); */

  msgdbg("loading sc68 [%s]\n",url);


  if (sc68_load_url(sc68,url))
    return -1;

  return 0;
}


/* static */
/* disk68_t * merge_disk(disk68_t * diska, disk68_t *  diskb) */
/* { */
/*   return 0; */
/* } */
