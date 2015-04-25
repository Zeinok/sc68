/*
 * @file    src68_par.c
 * @brief   memory partition.
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

#include "src68_par.h"
#include <stdlib.h>
#include <string.h>

static obj_t * ipart_new(void);

static int ipart_cmp(const obj_t * oa, const obj_t * ob)
{
  const par_t * sa = (const par_t *) oa;
  const par_t * sb = (const par_t *) ob;
  int res = sa->addr - sb->addr;
  if (!res)
    res = sa->size - sb->size;
  return res;
}

static void ipart_del(obj_t * obj)
{
  par_t * par = (par_t *) obj;
  free(par);
}

static objif_t if_part = {
  sizeof(par_t), ipart_new, ipart_del, ipart_cmp
};

static obj_t * ipart_new(void)
{
  return obj_new(&if_part);
}

vec_t * parts_new(unsigned int max)
{
  return vec_new(max,&if_part);
}

par_t * part_new(unsigned int addr, unsigned int size, unsigned int flag)
{
  par_t * par = (par_t *)ipart_new();
  if (par) {
    par->addr = addr;
    par->size = size;
    par->flag = flag;
  }
  return par;
}

int part_add(vec_t * parts,
             unsigned int addr, unsigned int size, unsigned int flag)
{
  int idx = -1;
  par_t * par = part_new(addr, size, flag);
  assert(parts);
  idx = vec_add(parts, &par->obj, VEC_FNONE);
  if (idx == -1)
    obj_del(&par->obj);
  return idx;
}

par_t * part_get(vec_t * parts, int idx)
{
  return (par_t *)vec_get(parts, idx);
}
