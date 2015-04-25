/*
 * @file    src68_mbk.c
 * @brief   memory blocks.
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (c) 1998-2015 Benjamin Gerard
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

#include "src68_mbk.h"
#include <stdlib.h>
#include <string.h>

mbk_t * mbk_new(unsigned int org, unsigned int len)
{
  mbk_t * mbk;
  unsigned int bytes = sizeof(mbk_t)-1+len*2;

  mbk = calloc(1,bytes);
  if (mbk) {
    mbk->org = org;
    mbk->len = len;
    mbk->mem = mbk->buf;
    mbk->mib = mbk->buf+len;
  }
  return mbk;
}

void mbk_free(mbk_t * mbk)
{
  if (mbk) {
    if (mbk->mem != mbk->buf)
      free(mbk->mem);
    if (mbk->mib != mbk->buf+mbk->len)
      free(mbk->mib);
    free(mbk);
  }
}

void mbk_clr(mbk_t * mbk)
{
  assert(mbk);
  memset(mbk->mib,0,mbk->len);
}

int mbk_ismine(const mbk_t * mbk, unsigned int addr)
{
  assert(mbk);
  return (addr >= mbk->org) && (addr < mbk->org+mbk->len);
}

