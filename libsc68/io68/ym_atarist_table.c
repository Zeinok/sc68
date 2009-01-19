/*
 *          sc68 - YM-2149 emulator - Atari ST Volume Table
 *             Copyright (C) 1999-2007 Benjamin Gerard
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 *
 */

/* $Id$ */

/* Table of 5 bit D/A output level for 1 channel.*/
/* static u16 volumetable[32 * 32 * 32]; */
static const u16 volumetable_original[16 * 16 * 16] =
#include "ym_fixed_vol.h"

static int
volumetable_get(int i, int j, int k)
{
  int v,v2;
  /* access at boundary finds the last value instead of the first */
  if (i == 16)
    i = 15;
  if (j == 16)
    j = 15;
  if (k == 16)
    k = 15;
  /* but calling with < 0 or > 16 should still trigger assert */
/*   assert(i >= 0 && i <= 15); */
/*   assert(j >= 0 && j <= 15); */
/*   assert(k >= 0 && k <= 15); */
    
    /* Close your eyes now... */
/*   i = 15 - i; */
/*   j = 15 - j; */
/*   k = 15 - k; */
  
  v = volumetable_original[i + 16 * j + 16 * 16 * k];
  v2 = (v*12)>>4;
/*   TRACE68(ym_feature,"v[%x %x %x] = %d->%d\n",k,j,i,v,v2); */

  return v2;
}

static void
volumetable_set(s16 * const volumetable, int i, int j, int k, int val)
{
/*   assert(i >= 0 && i <= 31); */
/*   assert(j >= 0 && j <= 31); */
/*   assert(k >= 0 && k <= 31); */
  
    /* Close your eyes now... Volumetable needs to be reversed for me
     * compared to the tabulated data I stole from someone else. */
/*   i = 31 - i; */
/*   j = 31 - j; */
/*   k = 31 - k; */
    
  val -= 32768;
  if (val < -32768) {
/*     TRACE68(ym_feature,"set [%02x %02x %02x] sature:%d\n",k,j,i,val); */
    val = -32768;
  }
  if (val > 32767) {
    TRACE68(ym_feature,"set [%02x %02x %02x] sature:%d\n",k,j,i,val);
    val = 32767;
  }

  volumetable[i + 32 * j + 32 * 32 * k] = val;
}

/* the table is exponential in nature. These weighing factors approximate
 * that the value in-between needs to be closer to the lower value in y2 */
static int
volumetable_interpolate(int y1, int y2)
{
  int erpolate;
  erpolate = (y1 * 4 + y2 * 6) / 10u;
  if (erpolate > 65535) {
    TRACE68(ym_feature, "sature>:%d %d %d\n",erpolate,y1,y2);
    erpolate = 65535;
  }
  if (erpolate < 0) {
    TRACE68(ym_feature, "sature<:%d %d %d\n",erpolate,y1,y2);
    erpolate = 0;
  }
/*   TRACE68(ym_feature, "erpole:%d <-%d %d\n",erpolate,y1,y2); */
/*   assert(erpolate >= volumetable_original[0] && erpolate <= volumetable_original[0xfff]); */
  return erpolate;
}

static void
interpolate_volumetable(s16 * const out)
{
  int i, j, k;
  int i1, i2;

  /* we are doing 4-dimensional interpolation here. For each
   * known measurement point, we must find 8 new values. These values occur
   * as follows:
   *
   * - one at the exact same position
   * - one half-way in i direction
   * - one half-way in j direction
   * - one half-way in k direction
   * - one half-way in i+j direction
   * - one half-way in i+k direction
   * - one half-way in j+k direction
   * - one half-way in i+j+k direction
   *
   * The algorithm currently is very simplistic. Probably more points should be
   * weighted in the multicomponented directions, for instance i+j+k should be
   * an average of all surrounding data points. This probably doesn't matter much,
   * though. This is because the only way to reach those locations is to modulate
   * more than one voice with the envelope, and this is rare.
   */

  for (i = 0; i < 16; i += 1) {
    for (j = 0; j < 16; j += 1) {
      for (k = 0; k < 16; k += 1) {
	i1 = volumetable_get(i, j, k);
	/* copy value unchanged to new position */
	volumetable_set(out,i*2, j*2, k*2, i1);
                
	/* interpolate in i direction */
	i2 = volumetable_get(i + 1, j, k);
	volumetable_set(out,i*2 + 1, j*2, k*2, volumetable_interpolate(i1, i2));
                
	/* interpolate in j direction */
	i2 = volumetable_get(i, j+1, k);
	volumetable_set(out,i*2, j*2 + 1, k*2, volumetable_interpolate(i1, i2));
                
	/* interpolate in k direction */
	i2 = volumetable_get(i, j, k+1);
	volumetable_set(out,i*2, j*2, k*2+1, volumetable_interpolate(i1, i2));

	/* interpolate in i + j direction */
	i2 = volumetable_get(i + 1, j + 1, k);
	volumetable_set(out,i*2 + 1, j*2 + 1, k*2, volumetable_interpolate(i1, i2));
                
	/* interpolate in i + k direction */
	i2 = volumetable_get(i + 1, j, k + 1);
	volumetable_set(out,i*2 + 1, j*2, k*2 + 1, volumetable_interpolate(i1, i2));
                
	/* interpolate in j + k direction */
	i2 = volumetable_get(i, j + 1, k + 1);
	volumetable_set(out,i*2, j*2 + 1, k*2 + 1, volumetable_interpolate(i1, i2));

	/* interpolate in i + j + k direction */
	i2 = volumetable_get(i + 1, j + 1, k + 1);
	volumetable_set(out,i*2 + 1, j*2 + 1, k*2 + 1, volumetable_interpolate(i1, i2));
      }
    }
  }
}

/* Create a linear 3 channels 5 bit per channels DAC table.
 */
void ym_create_5bit_atarist_table(s16 * out, unsigned int level)
{
  interpolate_volumetable(out);
  if (level) {
    int h;
    const int min = out[0x0000];
    const int max = out[0x7fff];
    const int div = max-min;
    const int center = level>>1;
    if (div>0) for (h=0; h<32*32*32; ++h) {
      int tmp = out[h], res;
      res = (tmp-min) * level / div - center;
      out[h] = res;
    }
  }
}
