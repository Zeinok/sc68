/*
 * @file    mksc68_snd.c
 * @brief   disk functions.
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (c) 1998-2014 Benjamin Gerard
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

#include "mksc68_snd.h"
#include "mksc68_msg.h"

/* #include "mksc68_cmd.h" */
/* #include "mksc68_tag.h" */
/* #include "mksc68_str.h" */
/* #include "mksc68_snd.h" */

#include <sc68/file68.h>
#include <sc68/file68_str.h>
#include <sc68/sc68.h>

/* #include <assert.h> */
/* #include <stdlib.h> */
/* #include <string.h> */
/* #include <stdio.h> */
/* #include <time.h> */
/* #include <errno.h> */
/* #include <unistd.h> */
/* #include <stdint.h> */

int sndh_save(const char * uri, void * _dsk, int version, int gzip)
{
  disk68_t * d = (disk68_t *)_dsk;

  msgerr("sndh: save '%s' version:%d compress:%d\n", uri, version, gzip);

  return -1;
}
